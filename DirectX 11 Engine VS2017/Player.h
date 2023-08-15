#pragma once

#include "Graphics/Model.h"

class Player
{
public:
	/*Player();
	~Player();*/
	int PlayerStateMachine = 0;//idle = 0 1 = jump up 2 = jump down
	bool inJumping;

	float Gravity = 1.0f;
	float MoveSpeed = 0.15f;

	float jumptime = 1.0f;
	float jumpspeed = 1.0f;
	
	float jumptimer = 0.0f;

	Model* model = NULL;

	void StartJump() 
	{
		if (PlayerStateMachine == 2 || inJumping == true) return;

		jumptimer = 0.0f;
		PlayerStateMachine = 1;
		inJumping = true;

		//change animation

		//start timer calculate jump;

	}

	void Update(float dt) 
	{
		if (PlayerStateMachine == 1)
		{
			jumptimer++;
			if (jumptimer > jumptime * 60)
			{
				//stop jump up turn to drop down
				PlayerStateMachine = 2;
				jumptimer = 0;
			}
			else
			{
				sinf(jumptimer / 2.0f * 3.0f / 180.0f) * jumpspeed;
			}
			
		}
		
	}

	
private:
	
};

//Player::Player()
//{
//}
//
//Player::~Player()
//{
//}