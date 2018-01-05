/* 
*   TimerManager 定时器管理器
*   1.定时器对象的定义
*   2.定时器的计时，触发
*   3.FixTime可以高效处理大量时间相等的定时器
*     例如每隔XX秒执行一次
*   4.AgileTime可以灵活处理各种时间不等的定时器
*     例如XX秒后执行一次
*     
*   2013-04-16 Create By 宝爷
*   2015-04-23 Refactor By 宝爷
*/
#ifndef __TIMERMANAGER_H__
#define __TIMERMANAGER_H__

#include <list>
#include <map>

#include "KxCore.h"
#include "KxTimeVal.h"

namespace KxServer {

class KxTimerList;

#define KXREPEAT_FROEVER    -1

// 时间超时对象，可继承重写onTimer回调
// 并注册到KxTimerManager中来开启计时任务
class KxTimerObject : public KxObject
{
    friend class KxTimerList;
    friend class KxTimerManager;
public:
    KxTimerObject();
    // delay表示需要等待的延迟时间，repeat表示要以delay为间隔重复执行多少次
    // repeat为0表示只执行一次
    KxTimerObject(float delay, int repeat = 0);
    virtual ~KxTimerObject();

    // 在计划的时间到达时会被触发
    virtual void onTimer(const kxTimeVal& now);

    // 停止计时
    void stop();

	// 超时返回true，用于检测超时时间
	inline bool checkTime(const kxTimeVal& now)
	{
        return m_TimeVal < now;
	}

    inline bool checkTime(float fnow)
	{
        return m_fTime < fnow;
	}

    inline int getRepeat()
    {
        return m_Repeat;
    }

    inline void setRepeat(int t)
    {
        m_Repeat = t;
    }

	inline float getDelay()
	{
		return m_Delay;
	}

    // 如需在运行中修改delay，请先stop再重新注册
    inline void setDelay(float d)
    {
        m_Delay = d;
    }
    
    inline float getTimeValFloat()
    {
        return m_fTime;
    }
    
    inline kxTimeVal& getTimeVal()
    {
        return m_TimeVal;
    }
private:
    // 开始计时，超时时间为now + delay，由KxTimerManager调用
    void start(const kxTimeVal& now);
    // 开始计时，超时时间为t，由KxTimerManager调用
    void startWithTime(const kxTimeVal& t);

private:
    KxTimerList* m_TimerList;
    KxTimerObject* m_Prev;
    KxTimerObject* m_Next;
    float m_Delay;              // 执行间隔
    int m_Repeat;               // 重复次数
    float m_fTime;              // float类型的超时时间
    kxTimeVal m_TimeVal;		// 超时时间
};

// 定时器链表
// 提供了高效的添加删除方法
class KxTimerList : public KxObject
{
public:
    KxTimerList();
    virtual ~KxTimerList();

    inline KxTimerObject* head()
    {
        return m_Head;
    }

    inline KxTimerObject* tail()
    {
        return m_Tail;
    }

    inline unsigned int length()
    {
        return m_Length;
    }

    bool pushBack(KxTimerObject* obj);

    bool pushFront(KxTimerObject* obj);

    bool remove(KxTimerObject* obj);

    bool insert(KxTimerObject* obj, float timeOut);

    void update(float fnow, const kxTimeVal& now);

private:
    unsigned int m_Length;
    KxTimerObject* m_Head;
    KxTimerObject* m_Tail;
    KxTimerObject* m_Timer;
};

// 定时器管理类
// 内部分为固定列表以及自由列表两种定时器列表
// 两种列表以定时器的时间间隔和重复次数进行划分
// 在插入与更新的效率上，固定列表要远高于自由列表，适合执行每隔XX秒触发的定时器
// 固定列表会为每一种时间间隔分配一个列表，如果这一时间间隔的定时器过少将造成资源的浪费
// 琐碎的时间间隔应该被添加到自由列表中
// 当你不确定应该添加到哪个列表时，或者期望添加到自由列表时
// 直接调用addTimer，让KxTimerManager帮你做最佳选择
class KxTimerManager : public KxObject
{
public:
    KxTimerManager();
    virtual ~KxTimerManager();

    // 更新所有的Timer
	void updateTimers();

    // 添加计时器，根究obj自身的delay和repeat来判断超时
    // 当repeat为0时，且没有与超时时间匹配的固定列表，将被添加到自由列表
    // 其它的情况将被添加到固定列表
    bool addTimer(KxTimerObject* obj);

    // 添加计时器，delay和repeat都会设置到obj身上
    // 当repeat为0时，且没有与超时时间匹配的固定列表，将被添加到自由列表
    // 其它的情况将被添加到固定列表
    bool addTimer(KxTimerObject* obj, float delay, int repeat = 0);
	
    // 明确指定添加到固定时长计时器列表
	bool attachToFixList(KxTimerObject* obj);
	
    // 明确指定添加到自由时长计时器列表
    bool attachToAglieList(KxTimerObject* obj);

    // 获取当前float时间
    inline float getFNow()
    {
        return m_fNow;
    }

    // 获取当前时间
    inline const kxTimeVal& getNow()
    {
        return m_Now;
    }

    // 将超时时间转换为Key，精度为万分之一秒
    inline long timeToKey(float delay)
    {
        return static_cast<long>(delay * 10000);
    }

    int getMillisecond()
    {
        return (m_Now.tv_sec * 1000) + (m_Now.tv_usec / 1000);
    }

private:
	float						    m_fNow;
	kxTimeVal					    m_Now;
	KxTimerList*         	        m_AglieTimerList;
	std::map<long, KxTimerList*>    m_FixTimerMap;	
};

}

#endif
