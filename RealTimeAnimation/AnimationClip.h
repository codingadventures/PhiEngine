#ifndef AnimationClip_h__
#define AnimationClip_h__


#include <string>
#include "AnimationPose.h"
#include "assimp\scene.h"
#include "Skeleton.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"

class AnimationClip
{
public:
	AnimationClip(double ,string ,string );
	AnimationClip(double , double, string );


	std::map<std::string,AnimationPose> mBoneMapping;

	std::string mAnimationName;
	double mTotalDuration;
	double mLocalTimer;
	double mAnimationSpeed; 
	AnimationPose* GetAnimationPose( string boneName);
	void SetAnimationPose(string boneName,AnimationPose animationPose);
	void Update(double deltaTime);
	void Reset(double animationSpeed = 0);

private: 

	void loadAnimations(string file_name );

	void Init();
};

void AnimationClip::Reset(double animationSpeed){
	 
	mLocalTimer = 0.0;

	if (animationSpeed != 0)
		mAnimationSpeed = animationSpeed;
}
void AnimationClip::loadAnimations(string file_name)
{
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_name,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	// Check for errors
	if(!scene || !scene->mRootNode) // if is Not Zero
	{
		printf("ERROR::ASSIMP::%s",importer.GetErrorString());
		return;
	}

	/* get the first animation out and into keys */
	if (scene->mNumAnimations > 0) {
		// get just the first animation
		aiAnimation* anim = scene->mAnimations[0];
		printf ("animation name: %s\n", anim->mName.C_Str ());
		printf ("animation has %i node channels\n", anim->mNumChannels);
		printf ("animation has %i mesh channels\n", anim->mNumMeshChannels);
		printf ("animation duration %f\n", anim->mDuration);
		printf ("ticks per second %f\n", anim->mTicksPerSecond);

		mTotalDuration = anim->mDuration;
		printf ("anim duration is %f\n", anim->mDuration);

		// get the node channels
		for (int i = 0; i < (int)anim->mNumChannels; i++) {
			AnimationPose lAnimationPose;

			aiNodeAnim* chan = anim->mChannels[i];
			// find the matching node in our skeleton by name
			/*	Bone* sn = skeleton->GetBone(
			chan->mNodeName.C_Str (),	NULL);
			if (!sn) {
			fprintf (stderr, "WARNING: did not find node named %s in skeleton."
			"animation broken.\n", chan->mNodeName.C_Str ());
			continue;
			}*/


			// add position keys to node
			for (int i = 0; i < chan->mNumPositionKeys; i++)  
				lAnimationPose.AddTranslationKeyFrame( aiVectorKeyToGlm(&chan->mPositionKeys[i]),chan->mPositionKeys[i].mTime);

			// add rotation keys to node
			for (int i = 0; i < chan->mNumRotationKeys; i++) 
				lAnimationPose.AddRotationKeyFrame(aiQuatKeyToGlm(&chan->mRotationKeys[i]),chan->mRotationKeys[i].mTime);

			// add scaling keys to node
			for (int i = 0; i < chan->mNumScalingKeys; i++) 
				lAnimationPose.AddScaleKeyFrame( aiVectorKeyToGlm(&chan->mScalingKeys[i]),chan->mScalingKeys[i].mTime);

			mBoneMapping[chan->mNodeName.C_Str()] = lAnimationPose;	
		} // end for mNumChannels
	} else {
		fprintf (stderr, "WARNING: no animations found in mesh file\n");
	} // endif mNumAnimations > 0

}

AnimationClip::AnimationClip(double animationSpeed, string file_name, string animationName) : mAnimationSpeed(animationSpeed), mAnimationName(animationName)
{
	Init();

	loadAnimations(file_name);
}

AnimationClip::AnimationClip(double animationSpeed, double totalDuration, string animationName)  : mAnimationSpeed(animationSpeed), mTotalDuration(totalDuration), mAnimationName(animationName)
{
	Init();
}

AnimationPose* AnimationClip::GetAnimationPose(std::string boneName)
{
	if (this->mBoneMapping.find(boneName) != this->mBoneMapping.end())
		return &this->mBoneMapping[boneName];

	return nullptr;
}

void AnimationClip::Init()
{
	mLocalTimer = 0.0; 
}

void AnimationClip::SetAnimationPose(string boneName,AnimationPose animationPose)
{
	mBoneMapping[boneName] = animationPose;
}

void AnimationClip::Update(double deltaTime)
{
	mLocalTimer += deltaTime / 1000  * mAnimationSpeed;

	if (mLocalTimer > mTotalDuration)
	{
		Reset();
		//signal animation is done.
	}
}

#endif // AnimationClip_h__