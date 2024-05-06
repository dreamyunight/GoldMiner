/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//
#include <Windows.h>
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
//
// 游戏总管类。负责处理游戏主循环、游戏初始化、结束等工作
class	CGameMain
{
private:
	int				m_iGameState;			// 游戏状态，0：结束或者等待开始；1：初始化；2：游戏进行中

    int             m_fGoldBornMinX;
	int	            m_fGoldBornMaxX;
	int	            m_fGoldBornMinY;
	int	            m_fGoldBornMaxY;
	int             m_iGoldCount;

	vector<CSprite*>golds;		            //管理金块，原理同海底世界中的rockFishs类似

	int				m_iGoldCoinCount;

	vector<CSprite*>goldCoins;				//管理金币，原理同上

	int             m_iDogCount;

	vector<CSprite*>Dogs;

	float			m_fHookRotation;		//钩子同地面的夹角
    int				m_iHookRotToLeft;		//钩子摆动的方向：1 ← ；0 →
    CSprite*		goldHook;			    //钩子对应的精灵

    int		        m_iHookState;	        //0 表示钩子旋转，1表示释放绳索，钩子不旋转,2表示回拉过程
    float			m_fHookSpeed;		    //钩子释放的速度
    CAnimateSprite*	goldMan;		        //矿工对应的Sprite

    float			m_fHookStartPosX;		//存储钩子的初始位置
    float			m_fHookStartPosY;
    CSprite*		m_gotGold;		        //存储抓到的金块

public:
	CGameMain();                            //构造函数
	~CGameMain();                           //析构函数

	// Get方法
	int				GetGameState()											{ return m_iGameState; }

	// Set方法
	void			SetGameState( const int iState )				{ m_iGameState	=	iState; }

	// 游戏主循环等
	void			GameMainLoop( float	fDeltaTime );
	void			GameInit();
	void			GameRun( float fDeltaTime );
	void			GameEnd();

	void			OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress );

	void			OnSpriteColSprite( const char *szSrcName, const char *szTarName );

	CSprite*		FindGoldSpriteByName(const char* szName);

	void			OnSpriteColWorldLimit( const char *szName, const int iColSide );

	CSprite*		FindSpriteByName(const char* szName);
};

/////////////////////////////////////////////////////////////////////////////////
//
extern CGameMain	g_GameMain;

#endif // _LESSON_X_H_
