//LeepMotionTest　機材とSDKが必要。
//プレゼンで使える。何も知らない上司の前で、フォースを操るようにスライドをめくる事により
//なんとか内容を忘れてくれる必殺技。効果はばつぐん。
//いろいろいじくってチャタリングも考慮しているので、
//指の動きを二重に探知して、ネタバレ２重めくりの危険性も減っている。

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


// onInit
void Listener::onInit( const Leap::Controller& controller )
{
}

// onConnect
void Listener::onConnect( const Leap::Controller& controller )
{
	// Enable Background App
	controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES); 

	
	// Enable detect Gesture
	controller.enableGesture( Leap::Gesture::TYPE_SWIPE );
	controller.enableGesture( Leap::Gesture::TYPE_SCREEN_TAP );
	controller.enableGesture( Leap::Gesture::TYPE_CIRCLE);
	controller.enableGesture( Leap::Gesture::TYPE_KEY_TAP);
}

// onDisconnect
void Listener::onDisconnect( const Leap::Controller& controller )
{
}

// onFrame
void Listener::onFrame( const Leap::Controller& controller )
{
	
		// Get Calibrated Screen List from Controller
	const Leap::ScreenList screenList = controller.locatedScreens();
	
	// Get First Calibrated Screen from Screen List
	if( screenList.isEmpty() ){
		std::cout << "Calibrated Screen Not Found" << std::endl;
		return;
	}
	screen = screenList[0];

	// Get Frame from Controller
	frame = controller.frame();

	// Get Hand List from Frame
	handList = frame.hands();

	// Get PointableList from First Hand List
	pointableList = handList[0].pointables();

	// Get First Pointable from Pointable List
	
	if (handList[0].isLeft()){
		if (!pointableList.isEmpty()){
			const Leap::Pointable pointable = pointableList[0];

			// Get Normalize Screen Intersect Position (Intersection of Screen and Destination of Pointable)
			const Leap::Vector intersect = screen.intersect(pointable, true, 1.0f);
			if (intersect.isValid()){
				// Set Position of Mouse Cursor
				unsigned int x = static_cast<unsigned int>(screen.widthPixels() * intersect.x);
				unsigned int y = static_cast<unsigned int>(screen.heightPixels() * (1.0f - intersect.y));

				SetCursorPos(x, y);
			}
		}
	}

	// Get GestureList from Frame
	const Leap::GestureList gestureList = frame.gestures();
	
	// Get Gesture from GestureList
	for( int count = 0; count < gestureList.count(); ++count ){
		Leap::Gesture gesture = gestureList[count];
		// Switch Gesture Type
		switch( gesture.type() ){

			// Case of Swipe Gesture
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

				// Vertical Swipe
				else{
					// Up Swipe
					if( swipeGesture.direction().y > 0.0f ){
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "↑" << std::endl;
							keybd_event( VK_UP, MapVirtualKey( VK_UP, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_UP, MapVirtualKey( VK_UP, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}

					// Down Swipe
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
	// Create a listener and controller
	Listener listener;
	Leap::Controller controller;

	// Have the listener receive events from the controller
	controller.addListener(listener);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the listener when done
	controller.removeListener(listener);

	return 0;
}
