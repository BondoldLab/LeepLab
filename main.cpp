//LeepMotionTest　機材とSDKが必要。
//プレゼンで使える。何も知らない上司の前で、フォースを操るようにスライドをめくる事により
//なんとか内容を忘れてくれる必殺技が使えるようになる。効果は実証済み。
//チャタリングも考慮しているので指の動きを二重に探知して、ネタバレ２重めくりの危険性も減っている。

#include "stdafx.h"
#include <Windows.h>
#include <Leap.h>

class Listener : public Leap::Listener
{
	public:
		virtual void onInit( const Leap::Controller& );
		virtual void onConnect( const Leap::Controller& );
		virtual void onDisconnect( const Leap::Controller& );
		virtual void onFrame( const Leap::Controller& );
		virtual void onExit( const Leap::Controller& );
		void showState(int hand);
	private:
		int CIRCLE = 0;
		int tap = 0;
		int key = 0;

		Leap::Screen screen;
		Leap::Frame frame;
		Leap::HandList handList;
		Leap::PointableList pointableList;
};

void Listener::showState(int hand)
{
	//テスト表示用
	if (hand){
		std::cout << "右手" << std::endl;
	}
	else{
		std::cout << "左手" << std::endl;
	}

	std::cout << "direction X,Y,Z:" << pointableList[hand].direction()<< std::endl;
	std::cout << "tipPosition X,Y,Z:" << pointableList[hand].tipPosition() << std::endl;
	std::cout << "tipVelcoity X,Y,Z:" << pointableList[hand].tipVelocity() << std::endl;

}


void Listener::onInit( const Leap::Controller& controller )
{
}

void Listener::onConnect( const Leap::Controller& controller )
{
	controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES); 
	
	controller.enableGesture( Leap::Gesture::TYPE_SWIPE );
	controller.enableGesture( Leap::Gesture::TYPE_SCREEN_TAP );
	controller.enableGesture( Leap::Gesture::TYPE_CIRCLE);
	controller.enableGesture( Leap::Gesture::TYPE_KEY_TAP);
}

void Listener::onDisconnect( const Leap::Controller& controller )
{
}

void Listener::onFrame( const Leap::Controller& controller )
{
	
	const Leap::ScreenList screenList = controller.locatedScreens();
	
	if( screenList.isEmpty() ){
		std::cout << "Calibrated Screen Not Found" << std::endl;
		return;
	}
	screen = screenList[0];

	frame = controller.frame();

	handList = frame.hands();

	pointableList = handList[0].pointables();

	
	if (handList[0].isLeft()){
		if (!pointableList.isEmpty()){
			const Leap::Pointable pointable = pointableList[0];

			const Leap::Vector intersect = screen.intersect(pointable, true, 1.0f);
			if (intersect.isValid()){
				unsigned int x = static_cast<unsigned int>(screen.widthPixels() * intersect.x);
				unsigned int y = static_cast<unsigned int>(screen.heightPixels() * (1.0f - intersect.y));

				SetCursorPos(x, y);
			}
		}
	}

	const Leap::GestureList gestureList = frame.gestures();
	
	for( int count = 0; count < gestureList.count(); ++count ){
		Leap::Gesture gesture = gestureList[count];
		
		switch( gesture.type() ){

			case Leap::Gesture::TYPE_SWIPE:
			{
				Leap::SwipeGesture swipeGesture = gesture;

				float direction = std::abs( swipeGesture.direction().x ) - std::abs( swipeGesture.direction().y );

				// Horizontal Swipe
				if( direction > 0.0f ){
					// Left Swipe
					if( swipeGesture.direction().x > 0.0f ){
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "→" << std::endl;
							keybd_event( VK_RIGHT, MapVirtualKey( VK_RIGHT, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_RIGHT, MapVirtualKey( VK_RIGHT, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}
					// Rite Swipe
					else{
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "←" << std::endl;
							keybd_event( VK_LEFT, MapVirtualKey( VK_LEFT, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_LEFT, MapVirtualKey( VK_LEFT, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}
				}

				else{
					if( swipeGesture.direction().y > 0.0f ){
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "↑" << std::endl;
							keybd_event( VK_UP, MapVirtualKey( VK_UP, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_UP, MapVirtualKey( VK_UP, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}

					else{
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "↓" << std::endl;
							keybd_event( VK_DOWN, MapVirtualKey( VK_DOWN, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_DOWN, MapVirtualKey( VK_DOWN, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}
				}
				break;
			}

			case Leap::Gesture::TYPE_CIRCLE:
			{	
				CIRCLE++;
				//実験用。シチュエーションが思い浮かばず断念。まず取れ方が不安定。多少精査しないとこのままでは使えない。
				//std::cout << "CIRCLE" << CIRCLE << std::endl;
				break;
			}

			// Case of KeyTap Gesture
			case Leap::Gesture::TYPE_KEY_TAP:
			{
				key++;
				std::cout << "KEY_TAP" << key << "　";
				showState(handList[0].isRight());
				//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				//sleepさせないとチャタリングが発生するので予防策。だいたい300がほど良い。
				Sleep(300);
				break;
			}

			// Case of ScreenTap Gesture
			case Leap::Gesture::TYPE_SCREEN_TAP:
			{
				tap++;
				std::cout << "SCREEN_TAP" << tap <<"　";
				showState(handList[0].isRight());
				//mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
				//mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
				break;
			}

			default:
			{
				break;
			}
		}
	}
}

// onExit
void Listener::onExit( const Leap::Controller& controller )
{
}

// Main
int _tmain(int argc, _TCHAR* argv[])
{
	Listener listener;
	Leap::Controller controller;

	controller.addListener(listener);

	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	controller.removeListener(listener);

	return 0;
}
