//LeepMotionTest�@�@�ނ�SDK���K�v�B
//�v���[���Ŏg����B�����m��Ȃ���i�̑O�ŁA�t�H�[�X�𑀂�悤�ɃX���C�h���߂��鎖�ɂ��
//�Ȃ�Ƃ����e��Y��Ă����K�E�Z���g����悤�ɂȂ�B���ʂ͎��؍ς݁B
//�`���^�����O���l�����Ă���̂Ŏw�̓������d�ɒT�m���āA�l�^�o���Q�d�߂���̊댯���������Ă���B

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
	//�e�X�g�\���p
	if (hand){
		std::cout << "�E��" << std::endl;
	}
	else{
		std::cout << "����" << std::endl;
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
							std::cout << "��" << std::endl;
							keybd_event( VK_RIGHT, MapVirtualKey( VK_RIGHT, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_RIGHT, MapVirtualKey( VK_RIGHT, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}
					// Rite Swipe
					else{
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "��" << std::endl;
							keybd_event( VK_LEFT, MapVirtualKey( VK_LEFT, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_LEFT, MapVirtualKey( VK_LEFT, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}
				}

				else{
					if( swipeGesture.direction().y > 0.0f ){
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "��" << std::endl;
							keybd_event( VK_UP, MapVirtualKey( VK_UP, 0 ), KEYEVENTF_EXTENDEDKEY, 0 );
							keybd_event( VK_UP, MapVirtualKey( VK_UP, 0 ), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
						}
					}

					else{
						if( swipeGesture.state() == Leap::Gesture::STATE_START ){
							std::cout << "��" << std::endl;
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
				//�����p�B�V�`���G�[�V�������v�������΂��f�O�B�܂��������s����B�����������Ȃ��Ƃ��̂܂܂ł͎g���Ȃ��B
				//std::cout << "CIRCLE" << CIRCLE << std::endl;
				break;
			}

			// Case of KeyTap Gesture
			case Leap::Gesture::TYPE_KEY_TAP:
			{
				key++;
				std::cout << "KEY_TAP" << key << "�@";
				showState(handList[0].isRight());
				//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				//sleep�����Ȃ��ƃ`���^�����O����������̂ŗ\�h��B��������300���قǗǂ��B
				Sleep(300);
				break;
			}

			// Case of ScreenTap Gesture
			case Leap::Gesture::TYPE_SCREEN_TAP:
			{
				tap++;
				std::cout << "SCREEN_TAP" << tap <<"�@";
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
