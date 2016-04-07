#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle
{
private:
	bool m_alive;

	Vector2f m_position;
	Vector2f m_velocity;
	Sprite m_sprite;
	Texture m_texture;

	Texture m_radarTexture;
	Sprite m_radarSprite;

	int m_fullWidth;
	int m_fullHeight;

	int m_speed;

	bool m_inRangeOfBoid;
	bool m_inRangeOfFactory;
	bool m_inRangeOfPredator;

public:
	Obstacle();// Default constructor
	Obstacle(float xPos, float yPos, float angle, int fullWidth, int fullHeight);

	void Draw(RenderWindow &window);
	void DrawOnRadar(RenderWindow &window);
	void Update();

	void SetAlive(bool myAlive);
	bool GetAlive();

	Sprite GetSprite();
	Vector2f GetPosition();
	Vector2f GetVelocity();

	vector<Obstacle*> CreateObstacle(int width, int height, Vector2f playerPos, vector<Obstacle*> obstacles);

	bool getInRangeOfBoid();
	void setInRangeOfBoid(bool myInRangeOfBoid);

	bool getInRangeOfFactory();
	void setInRangeOfFactory(bool myInRangeOfFactory);

	bool getInRangeOfPredator();
	void setInRangeOfPredator(bool myInRangeOfPredator);

};

#endif