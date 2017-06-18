#include "YellowTank.h"

USING_NS_CC;

using namespace cocos2d;

YellowTank::YellowTank() {}

YellowTank::~YellowTank() {}

YellowTank* YellowTank::create()
{
    YellowTank* pSprite = new YellowTank();

    if (pSprite->initWithSpriteFrameName("yellowAttack1.png"))
    {
        pSprite->autorelease();
        pSprite->initOptions();
        pSprite->addEvents();
        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return nullptr;
}

void YellowTank::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
    auto tankScaleSize = 0.7;

    this->setAnchorPoint(Point(0.5, 0.5));
    this->setPosition(Director::getInstance()->getVisibleSize().width - 100, 100);
    this->setScale(tankScaleSize);
    this->setFlipX(true);

    // set �������ԣ����Ӹ���
}

void YellowTank::addEvents() {
    this->addTouchListener();
}

void YellowTank::addTouchListener()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // ��ʼ���µĻص�����
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {   
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();

        if(rect.containsPoint(p))
        {
            m_power = 0;
            m_istouch = true;//��ť����
            this->schedule(schedule_selector(YellowTank::updatePowerbar), 0.1);//����ʱ���жϣ�ÿ��0.1�����һ��

            // ���µ�ʱ���������Ƚ���������������ȥ
            // ����������������label����
            powerbar = Label::createWithTTF("0", "fonts/arial.ttf", 20);
            // �����̹�������ö�Ӧ��powerbarλ��
            powerbar->setPosition(this->getPosition().x, this->getPosition().y + 100);
            this->getParent()->addChild(this->powerbar);

            return true; // to indicate that we have consumed it.
        }
        return false; // we did not consume this event, pass thru.
    };

    // �������µĻص�����
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        // ����������ȥ��
        this->unschedule(schedule_selector(YellowTank::updatePowerbar));
        this->getParent()->removeChild(powerbar);

        CCLOG("launch _power:%f", m_power);
        YellowTank::touchEvent(touch);
        m_istouch = false;
    };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void YellowTank::touchEvent(cocos2d::Touch* touch)
{
    this->runAttack();
    CCLOG("touched YellowTank");
}

void YellowTank::updatePowerbar(float ft)
{
    if (m_istouch)
    {
        // ���������ٶ������������ҷ�����UI����
        m_power += m_pressv;
        auto temp = std::to_string(m_power);
        powerbar->setString(temp);
    }
}

void YellowTank::runAttack()
{
    auto fireAnimate = CallFunc::create([&]() {
        // �����������Ǹ������boom�Ķ��������ú�λ�ã����ӣ����ж������Ƴ�
    });

    auto launch = CallFunc::create([&]() {
        // �����������ӵ�����,ͬʱ���ú������ĸ������ԣ���ת����Ƕȣ�ˮƽ�ʹ�ֱ���ٶȵȵȣ�
        // ����m_power���ú��ӵ���ɱ��������FireMaster������������е��ȼ����ײ��
    });

    auto attackAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("yellowTankAttackAnimation"));

    auto s = Sequence::create(attackAnimate, fireAnimate, launch, nullptr);
    this->runAction(s);
}