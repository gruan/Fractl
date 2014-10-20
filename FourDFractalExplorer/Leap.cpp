#ifndef LEAFCPP
#define LEAFCPP 1

//
//  main.cpp
//  Leap
//
//  Created by George Ruan on 10/4/14.
//  Copyright (c) 2014 George Ruan. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <math.h>
#include <Leap.h>

#define SWIPETOLERANCE 500
#define MAXROTATION 200.f
#define MINTIP 50
#define MINTIPVELOCITY 150

#define MINA -2.f
#define MINB -2.f
#define MINC -2.f
#define MIND -2.f

#define MAXA 2.f
#define MAXB 2.f
#define MAXC 2.f
#define MAXD 2.f



class FracListener : public Leap::Listener {
public:
	float LEAPdistance(Leap::Vector v1, Leap::Vector v2) {
		return sqrtf( powf((v1.x-v2.x), 2) + powf((v1.z - v2.z), 2));
	}

	FracListener(){
		a = b = c = d = 0.5f;
		radius = 60.;
		xRotation = yRotation = 0.01f;
	}

	float a;
	float b;
	float c;
	float d;
	float radius;
	float xRotation;
	float yRotation;
	virtual void onConnect(const Leap::Controller& controller) {
		std::cout << "Connected" << std::endl;
		controller.config().save();
	};

	virtual void onFrame(const Leap::Controller& controller){
		
		const Leap::Frame frame = controller.frame();


		Leap::HandList hands = frame.hands();

		if(hands.count()==2){
			Leap::Hand left = hands.leftmost();
			Leap::Hand right = hands.rightmost();

			Leap::Finger f1l = left.fingers()[1];
			Leap::Finger f2l = left.fingers()[2];
			Leap::Finger f3l = left.fingers()[3];
			Leap::Finger f4l = left.fingers()[4];

			Leap::Finger f1r = right.fingers()[1];
			Leap::Finger f2r = right.fingers()[2];
			Leap::Finger f3r = right.fingers()[3];
			Leap::Finger f4r = right.fingers()[4];

			Leap::Vector p1l = f1l.tipPosition();
			Leap::Vector p2l = f2l.tipPosition();
			Leap::Vector p3l = f3l.tipPosition();
			Leap::Vector p4l = f4l.tipPosition();

			Leap::Vector p1r = f1r.tipPosition();
			Leap::Vector p2r = f2r.tipPosition();
			Leap::Vector p3r = f3r.tipPosition();
			Leap::Vector p4r = f4r.tipPosition();

			float d1 = LEAPdistance(p1l, p1r);
			float d2 = LEAPdistance(p2l, p2r);
			float d3 = LEAPdistance(p3l, p3r);
			float d4 = LEAPdistance(p4l, p4r);

			std::cout << "a = " << a << "\n" << "b = " << b << "\n"<< "c = " << c << "\n"<< "d = " << d << "\n";

			if(d1 < MINTIP && d2 > MINTIP && d3 > MINTIP && d4 > MINTIP) {
				if(abs(f1l.tipVelocity().z) > MINTIPVELOCITY) {

					if(a + f1l.tipVelocity().z/7500 > MAXA) {
						a = MAXA;
					}

					if(a + f1l.tipVelocity().z/7500 < MINA) {
						a = MINA;
					}
					else {
						a += f1l.tipVelocity().z/7500;
					}
				}
			}

			if(d2 < MINTIP && d1 > MINTIP && d3 > MINTIP && d4 > MINTIP) {
				if(abs(f2l.tipVelocity().z) > MINTIPVELOCITY) {

					if(b + f2l.tipVelocity().z/7500 > MAXB) {
						b = MAXB;
					}

					if(b + f2l.tipVelocity().z/7500 < MINB) {
						b = MINB;
					}
					else {
						b += f2l.tipVelocity().z/7500;
					}
				}
			}

			if(d3 < MINTIP && d2 > MINTIP && d1 > MINTIP && d4 > MINTIP) {
				if(abs(f3l.tipVelocity().z) > MINTIPVELOCITY) {

					if(c + f3l.tipVelocity().z/7500 > MAXC) {
						c = MAXC;
					}

					if(c + f3l.tipVelocity().z/7500 < MINC) {
						c = MINC;
					}
					else {
						c += f3l.tipVelocity().z/7500;
					}
				}
			}

			if(d4 < MINTIP && d2 > MINTIP && d3 > MINTIP && d1 > MINTIP) {
				if(abs(f4l.tipVelocity().z) > MINTIPVELOCITY) {

					if(d + f4l.tipVelocity().z/7500 > MAXD) {
						d = MAXD;
					}

					if(d + f4l.tipVelocity().z/7500 < MIND) {
						d = MIND;
					}
					else {
						d += f4l.tipVelocity().z/7500;
					}
				}
			}

		}

		//Fractal in Hands

		Leap::Vector leftNormal = hands.leftmost().palmNormal();
		Leap::Vector rightNormal = hands.rightmost().palmNormal();

		float xHandsOpposing = leftNormal.x + rightNormal.x;
		float zHandsOpposing = leftNormal.z + rightNormal.z;

		bool handsOpposing = (abs(xHandsOpposing) - abs(zHandsOpposing)) < 0.02;

		if(hands.count() == 2 && handsOpposing) {
			Leap::Vector leftHand = hands.leftmost().palmPosition();
			Leap::Vector rightHand = hands.rightmost().palmPosition();

			float xDistance = leftHand.x - rightHand.x;
			float yDistance = leftHand.y - rightHand.y;
			float zDistance = leftHand.z - rightHand.z;

			float distance = sqrtf(powf(xDistance, 2) + powf(yDistance, 2) + powf(zDistance, 2));

			radius = distance / 2;

			std::cout << radius << "\n";
		}


		//Rotational Movement of Fractal
		if(hands.count() == 1) {
			for(Leap::Hand hand:hands) {

				Leap::Vector handSpeed = hand.palmVelocity();

				if(abs(handSpeed.x) > SWIPETOLERANCE || abs(handSpeed.y) > SWIPETOLERANCE || abs(handSpeed.z) > SWIPETOLERANCE) {

					//            std::cout << hand << + ": ";
					//            if(hand.isRight())
					//                std::cout << "Right Hand ";
					//            else
					//                std::cout << "Left Hand ";

					std::cout << "\n" << handSpeed;

					float a = abs(handSpeed.x);
					float b = abs(handSpeed.y);
					float c = abs(handSpeed.z);

					//Determines Swipe

					if(a >= b && a >= c) {
						if(handSpeed.x > 0) {
							std::cout << "\n Left-Swipe \n";
						}
						else {
							std::cout << "\n Right-Swipe \n";}
					}


					if (b >= a && b >= c) {
						if(handSpeed.y > 0) {
							std::cout << "\n Away-You-Swipe \n";
						}
						else {
							std::cout << "\n Towards-You-Swipe \n";
						}
					}

					if (c >= a && c >= b) {
						if(handSpeed.z > 0) {
							std::cout << "\n Down-Swipe \n";
						}
						else {
							std::cout << "\n Up-Swipe \n";
						}
					}

					//Adds to global Rotation

					if(a > SWIPETOLERANCE) {
						if(abs(xRotation + (handSpeed.x/300)) > MAXROTATION) {
							xRotation =  xRotation > 0?MAXROTATION:-MAXROTATION;
						}
						else {
							xRotation += (handSpeed.x/300);
						}
					}

					if(c > SWIPETOLERANCE) {
						if(abs(yRotation + (handSpeed.z/300)) > MAXROTATION) {
							yRotation = yRotation > 0?MAXROTATION: -MAXROTATION;
						}
						else {
							yRotation += (handSpeed.z/300);

						}
					}



					std::cout << xRotation << "  " << yRotation;
				}

			}
		}

	};

};
#endif