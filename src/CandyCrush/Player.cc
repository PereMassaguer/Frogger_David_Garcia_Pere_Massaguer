#include "Player.hh"



Player::Player()
{
	ResetTransform();
	sprite.objectID = ObjectID::FrogIUp;
	float acumulateX = 0.0f;
}

Player::~Player()
{
}

void Player::MoveUp()
{	
	if (sprite.transform.y >= GRID_Y_DISPLACEMENT + CELL_HEIGHT)
		sprite.transform.y -= CELL_HEIGHT;
		sprite.objectID = ObjectID::FrogIUp;
	//setspriteID
}

void Player::MoveDown()
{
	if (sprite.transform.y <= GRID_Y_DISPLACEMENT + (GRID_HEIGHT - 2) * CELL_HEIGHT) {
		sprite.transform.y += CELL_HEIGHT;
		sprite.objectID = ObjectID::FrogIDown;
		if (sprite.transform.y == GRID_Y_DISPLACEMENT + CELL_HEIGHT * 5) CenterToClosestCell();
	}
}

void Player::MoveLeft()
{
	if (sprite.transform.x >= CELL_WIDTH)
		sprite.transform.x -= CELL_WIDTH;
		sprite.objectID = ObjectID::FrogILeft;
}

void Player::MoveRight()
{
	if (sprite.transform.x <= (GRID_WIDTH - 2) * CELL_WIDTH)
		sprite.transform.x += CELL_WIDTH;
		sprite.objectID = ObjectID::FragIRight;
}


void Player::Draw()
{	
	sprite.Draw();
}

void Player::Update(float velocity)
{
	int row = GetGridCoords().second;
	if (row < 5) {
		acumulateX += TM.GetDeltaTime() / 30.0f * velocity;
		if (acumulateX >= 1.0f) {
			sprite.transform.x += (row % 2 == 0) ? (int)acumulateX : (int)-acumulateX;
			acumulateX -= (int)acumulateX;
		}
	}
	if (sprite.transform.x < 0)sprite.transform.x = 0;
	if (sprite.transform.x > W.GetWidth() - CELL_WIDTH) sprite.transform.x = W.GetWidth() - CELL_WIDTH;
}

void Player::Reset()
{
	ResetTransform();
	sprite.objectID = ObjectID::FrogIUp;
	float acumulateX = 0.0f;
}

Coord Player::GetGridCoords()
{
	Coord ret;
	ret.first = sprite.transform.x / CELL_WIDTH;
	ret.second = (sprite.transform.y - GRID_Y_DISPLACEMENT) / CELL_HEIGHT;

	if (sprite.transform.x % CELL_WIDTH > CELL_WIDTH / 2) ret.first++;

	return ret;
}


Coord Player::GetTransformCoords()
{
	return Coord(sprite.transform.x, sprite.transform.y);
}

void Player::ResetTransform()
{
	sprite.transform = { (GRID_WIDTH / 2) * CELL_WIDTH, (GRID_HEIGHT - 1) * CELL_HEIGHT + GRID_Y_DISPLACEMENT, 30, 30 };
}

void Player::CenterToClosestCell()
{
	if (sprite.transform.x % CELL_WIDTH > CELL_WIDTH / 2)
		sprite.transform.x = ((sprite.transform.x / CELL_WIDTH) + 1) * CELL_WIDTH;
	else { sprite.transform.x = (sprite.transform.x / CELL_WIDTH) * CELL_WIDTH; }
}