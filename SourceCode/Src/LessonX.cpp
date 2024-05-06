/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include <vector>
#include "CommonClass.h"
#include "LessonX.h"
#include <vld.h>
////////////////////////////////////////////////////////////////////////////////
//
//
CGameMain		g_GameMain;

//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。

//==============================================================================
//
// 构造函数
CGameMain::CGameMain()
{
	_CrtSetBreakAlloc(106);//

    goldHook            = new CSprite("goldhook");		//创建钩子实例
    goldMan             = new CAnimateSprite("goldMan");
	m_iGameState		=	1;
}
//==============================================================================
//
// 析构函数
CGameMain::~CGameMain()
{
	free(goldHook);
	free(goldMan);	
	for (auto it : golds) {
		free(it);
	}
	for (auto it : goldCoins) {
		free(it);
	}
	for (auto it : Dogs) {
		free(it);
	}
}

//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态.
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void CGameMain::GameMainLoop( float	fDeltaTime )
{
	switch( GetGameState() )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			SetGameState(2); // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else // 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
			{
				SetGameState(0);
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}
//=============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void CGameMain::GameInit()
{
    m_iGoldCount		=	30;	//金子初始数量
	m_iGoldCoinCount	=	5;	//金币数量
	m_iDogCount         =   3;  //狗数量

	m_fGoldBornMinX		=	CSystem::GetScreenLeft() + 5;		//金子左边界
	m_fGoldBornMaxX		=	CSystem::GetScreenRight() - 5;		//金子右边界
	m_fGoldBornMinY		=	CSystem::GetScreenTop() + 20;		//金子上边界
	m_fGoldBornMaxY		=	CSystem::GetScreenBottom() -5;		//金子下边界

    m_iHookRotToLeft    =	1;					    //钩子初始化方向为←
	m_fHookRotation     =	0.f;					//初始化的夹角为0

	m_iHookState        =	0;		                //初始情况下使钩子旋转
    m_fHookSpeed        =	15.f;

    m_fHookStartPosX    =	goldHook->GetSpritePositionX();	//获取钩子的初始化X坐标
	m_fHookStartPosY    =	goldHook->GetSpritePositionY();//获取钩子的初始化Y坐标

	int	iLoop	        =	0;	                    //循环变量控制
	int	iSize			=	4,		iPosX			=	0,		iPosY			=	0;//iSize表示金块大小的变量
	for( iLoop = 0; iLoop < m_iGoldCount; iLoop++ )	{
		if( iLoop < 15 )		                    //生成10个小金块，大小为4
		{
			iSize	    =	4;
		}
		else if( iLoop >= 15 && iLoop < 24 )	    //生成6个中金块，大小为6
		{
			iSize	    =	6;
		}
		else			//生成4个大金块，大小为8
		{
			iSize	    =	8;
		}
		//初始化金子精灵实例
		char* tmpName;
		//生成金块名字
		tmpName=CSystem::MakeSpriteName("GoldBlock",iLoop);
		CSprite *tmpSprite = new CSprite(tmpName);
		tmpSprite->CloneSprite("goldTemplate");
		tmpSprite->SetSpriteWidth(iSize);			//设置金块的宽度
		tmpSprite->SetSpriteHeight(iSize);			//设置金块的高度
		//随机的获取金子的X坐标和Y坐标
		iPosX	        =	CSystem::RandomRange(m_fGoldBornMinX, m_fGoldBornMaxX);
		iPosY	        =	CSystem::RandomRange(m_fGoldBornMinY, m_fGoldBornMaxY);
         //设置金块精灵的位置
		tmpSprite->SetSpritePosition(iPosX, iPosY);
		golds.push_back(tmpSprite);	                //将金块压入golds vector中集中管理
	}
	for( iLoop = 0; iLoop < m_iGoldCoinCount; iLoop++ ) {
		//初始化金币精灵实例
		char* tmpName;
		//生成金币名字
		tmpName=CSystem::MakeSpriteName("GoldCoin", iLoop);
		CSprite* tmpSprite = new CSprite(tmpName);
		tmpSprite->CloneSprite("goldCoin");
		//随机的获取金币的X坐标和Y坐标
		iPosX = CSystem::RandomRange(m_fGoldBornMinX, m_fGoldBornMaxX);
		iPosY = CSystem::RandomRange(m_fGoldBornMinY, m_fGoldBornMaxY);
		//设置金币精灵的位置
		tmpSprite->SetSpritePosition(iPosX, iPosY);
        //将金币压入goldCoin vector中集中管理
		goldCoins.push_back(tmpSprite);
	}
	for( iLoop = 0; iLoop < m_iDogCount; iLoop++ ){
		//初始化狗精灵实例
        char* tmpName;
		//生成狗名字
        tmpName=CSystem::MakeSpriteName("Dog", iLoop);
        CSprite* tmpSprite = new CSprite(tmpName);
		tmpSprite->CloneSprite("dog");
		//随机的获取狗的X坐标和Y坐标
		iPosX = CSystem::RandomRange(m_fGoldBornMinX, m_fGoldBornMaxX);
		iPosY = CSystem::RandomRange(m_fGoldBornMinY, m_fGoldBornMaxY);
		//设置狗精灵的位置
		tmpSprite->SetSpritePosition(iPosX, iPosY);
		//设置狗精灵向右移动
		tmpSprite->SetSpriteLinearVelocity(-12, 0);
        //将金币压入Dogs vector中集中管理
		Dogs.push_back(tmpSprite);
	}
}
//=============================================================================
//
// 每局游戏进行中
void CGameMain::GameRun( float fDeltaTime )
{
	if(m_iHookState ==0){		                    //当m_iHookState==0时钩子才摆动
        const float	fRotateSpeed = 45.f; 			//  摇摆速度，单位 度/秒
        float fThisRotate =	fRotateSpeed * fDeltaTime;  // 本次旋转的角度
        if( m_iHookRotToLeft ){					    //向左转，度数不断变大
            m_fHookRotation	+= fThisRotate;
            if( m_fHookRotation >= 180.f ){		    //大于180，置为向右转，即0
                m_fHookRotation		=	180.f;
                m_iHookRotToLeft	=	0;
            }
        }
        else{		//向右转，度数不断变小
            m_fHookRotation	-= fThisRotate;		    //小于0，置为向左转，即 1
            if( m_fHookRotation <= 0.f ){
                m_fHookRotation		=	0.f;
                m_iHookRotToLeft	=	1;
            }
        }
        goldHook->SetSpriteRotation(m_fHookRotation);//设置钩子的当前角度
	}else if(m_iHookState == 1 || m_iHookState == 2){		            //当钩子伸出后才需要画线
		// 首先，从矿工精灵上获取一个缆绳链接点作为绳子的起始点(该链接点在编辑器里编辑好)
		float fStartX =	goldMan->GetSpriteLinkPointPosX(1);
		float fStartY = goldMan->GetSpriteLinkPointPosY(1);
		// 绳子终点在钩子精灵上获取(该链接点在编辑器里编辑好)
		float fEndX	= goldHook->GetSpriteLinkPointPosX(1);
		float fEndY	= goldHook->GetSpriteLinkPointPosY(1);
		// 在这两点之间划线.线的颜色红绿蓝值都为50，即灰色；255表示不透明，2.0f表示线的粗细，0表示所在的层
		CSystem::DrawLine(fStartX, fStartY, fEndX, fEndY, 2.f, 0, 50, 50, 50, 255 );
        if(m_iHookState == 2){		//回拉过程中
			float	fSpeedX	=	goldHook->GetSpriteLinearVelocityX();	//获取钩子X方向的速度
			float	fSpeedY	=	goldHook->GetSpriteLinearVelocityY();	//获取钩子Y方向的速度
			//当速度接近为0时，即可判定其已到达初始点
			if( fSpeedX < 0.00001f && fSpeedX > -0.00001f && fSpeedY < 0.00001f && fSpeedY > -0.00001f ){
				m_gotGold->SpriteDismount();	//解除金块与钩子的锚定
				m_gotGold->DeleteSprite();		//删除获取的金块
				m_iHookState = 0;				//回拉结束，设定钩子状态为0,继续来回摆动
				goldMan->AnimateSpritePlayAnimation("GolderManAnimation2", false);	//播放矿工的动画，即准备拉金子的动画
			}
		}
	}
}
//=============================================================================
//
// 本局游戏结束
void CGameMain::GameEnd()
{
}

void CGameMain::OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress ){
	if( KEY_DOWN == iKey &&  m_iHookState == 0){		//按下↓并且游戏状态为	“1”
		m_iHookState = 1;		//置游戏状态为1，可用于控制钩子是否摆动
		// 以当前朝向给钩子一个向前的速度
		goldHook->SetSpriteLinearVelocityPolar(m_fHookSpeed, m_fHookRotation);
		// 播放挖金者的动作(一个胳膊往下压的动作)
		goldMan->AnimateSpritePlayAnimation("GolderManAnimation1", 0);	//0 表示播放一次，这里胳膊往下压就是一次
	}
}

void CGameMain::OnSpriteColSprite( const char *szSrcName, const char *szTarName ){
    if(strcmp(szSrcName, "goldhook")     == 0 &&
       (strstr(szTarName, "GoldBlock")   !=NULL ||
        strstr(szTarName, "Dog")         !=NULL ||
        strstr(szTarName, "GoldCoin")    !=NULL
        )){
        m_gotGold = FindGoldSpriteByName(szTarName);	//在golds中查找是否存在对应的金块，并返回CSprite*指针指向该金块
            if(m_gotGold!=NULL){
            //思考：为什么链接点的位置为2？
            m_gotGold->SpriteMountToSpriteLinkPoint("GoldHook", 2 );	//将金块锚定在钩子上
            goldHook->SpriteMoveTo(m_fHookStartPosX, m_fHookStartPosY, m_fHookSpeed, 1);	//使钩子向初始位置移动，即回拉
            //思考：AnimateSpritePlayAnimation中的false参数的作用是什么？
            goldMan->AnimateSpritePlayAnimation("GolderManAnimation3", false );	//播放拉金块的动作
            m_iHookState = 2;		//表示金块回拉的状态
        }
	}
}


CSprite* CGameMain::FindGoldSpriteByName(const char* szName)
{
    int iLoop;
	for(int iLoop=0;iLoop<golds.size();iLoop++)
		if(strcmp(szName,golds[iLoop]->GetName())==0)
			return golds[iLoop];
    for(int iLoop=0;iLoop<goldCoins.size();iLoop++)
		if(strcmp(szName,goldCoins[iLoop]->GetName())==0)
			return goldCoins[iLoop];
    for(int iLoop=0;iLoop<Dogs.size();iLoop++)
		if(strcmp(szName,Dogs[iLoop]->GetName())==0)
			return Dogs[iLoop];
	return NULL;
}

void CGameMain::OnSpriteColWorldLimit( const char *szName, const int iColSide ){
	CSprite* tmp;
	if(strcmp(szName,"goldhook") == 0){		//碰到边界的为钩子时
			goldHook->SpriteMoveTo(m_fHookStartPosX, m_fHookStartPosY, m_fHookSpeed, 1);
		goldMan->AnimateSpritePlayAnimation("GolderManAnimation3", false );
		m_iHookState = 2;	//同样设置为回拉，只是没有碰到金子
	}
	if((tmp = FindSpriteByName(szName))!=NULL){
        if(iColSide == 1){
			tmp->SetSpriteLinearVelocityX(-12.f);
			tmp->SetSpriteFlipX(false);
		}else if(iColSide == 0){
			tmp->SetSpriteLinearVelocityX(12.f);
			tmp->SetSpriteFlipX(true);
		}
    }
}

CSprite* CGameMain::FindSpriteByName(const char* szName)
{
    for(int i=0;i<Dogs.size();i++)			//size()返回vector的大小
        if(strcmp(szName,Dogs[i]->GetName())==0)	//遍历vector查找与szName具有相同的名称的Dogs中的精灵
            return Dogs[i];
    return NULL;
}
