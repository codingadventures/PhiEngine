#include <glm/glm.hpp>

struct IKInfo
{
	float cosAngle;
	float distance;
	float degreeAngle;
	glm::vec3 crossProduct;
	glm::vec3 currentWorldPosition;
	glm::vec3 effectorWorldPosition;
	int iteration;
	string currBoneName;
	glm::vec3 boneSpaceCrossProduct;
	IKInfo(){
		iteration	= 0;
		cosAngle	= 0;
		degreeAngle = 0;
		distance	= 0;
	}
};