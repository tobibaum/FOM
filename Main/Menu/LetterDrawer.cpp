/*
 * LetterDrawer.cpp
 *
 *  Created on: May 17, 2011
 *      Author: hendrik
 */

#include "LetterDrawer.h"

using namespace std;

LetterDrawer::LetterDrawer() {
	// TODO Auto-generated constructor stub

}

LetterDrawer::~LetterDrawer() {
	// TODO Auto-generated destructor stub
}

btCompoundShape* LetterDrawer::getLetters(string letters){
	LetterCompoundShape* cshape = new LetterCompoundShape();
	cshape->setType(letters);


	list<string> wordlist;
	if(true){
		string word = "";
		for(int i = 0; i<letters.size(); i++){
			char currentChar = letters.at(i);
			if(!isspace(currentChar) && (i != (letters.size()-1))){
				word += currentChar;
			}
			else{
				if(i == (letters.size()-1)){word += currentChar;}
				wordlist.push_back(word);
				word = "";
			}

		}
	}
//	printf("   %i\n",wordlist.size());
	btVector3 Yoffset = btVector3(0,(wordlist.size()-1)*5,0);


	list<string>::iterator it;
	for(it = wordlist.begin(); it != wordlist.end(); it++){

		btVector3 offset = btVector3(-((float)(it->size()*5))/2,0,0);
		int letterOffset=0;
		offset = offset + Yoffset;
		for(int i = 0; i < it->size(); i++){
			char currentChar = it->at(i);
//			printf("%c\n",currentChar);

			switch(currentChar){

			case 'A': addA(cshape, offset); letterOffset=5.5;
					break;
			case 'B': addB(cshape, offset); letterOffset=4.5;
					break;
			case 'C': addC(cshape, offset); letterOffset=5;
					break;
			case 'D': addD(cshape, offset); letterOffset=5;
					break;
			case 'E': addE(cshape, offset); letterOffset=5;
					break;
			case 'F': addF(cshape, offset); letterOffset=5;
					break;
			case 'L': addL(cshape, offset); letterOffset=4.5;
					break;
			case 'G': addG(cshape, offset); letterOffset=5;
					break;
			case 'H': addH(cshape, offset); letterOffset=6;
					break;
			case 'I': addI(cshape, offset); letterOffset=3;
					break;
			case 'J': addJ(cshape, offset); letterOffset=5;
					break;
			case 'K': addK(cshape, offset); letterOffset=5;
					break;
			case 'M': addM(cshape, offset); letterOffset=6;
					break;
			case 'N': addN(cshape, offset); letterOffset=6;
					break;
			case 'O': addO(cshape, offset); letterOffset=6;
					break;
			case 'P': addP(cshape, offset); letterOffset=4.5;
					break;
			case 'Q': addQ(cshape, offset); letterOffset=6;
					break;
			case 'R': addR(cshape, offset); letterOffset=4.5;
					break;
			case 'S': addS(cshape, offset); letterOffset=5;
					break;
			case 'T': addT(cshape, offset); letterOffset=5;
					break;
			case 'U': addU(cshape, offset); letterOffset=6;
					break;
			case 'V': addV(cshape, offset); letterOffset=6;
					break;
			case 'W': addW(cshape, offset); letterOffset=6;
					break;
			case 'X': addX(cshape, offset); letterOffset=5;
					break;
			case 'Y': addY(cshape, offset); letterOffset=6;
					break;
			case 'Z': addZ(cshape, offset); letterOffset=6;
					break;

			/****** EXTRA ELEMENTS *******/
			case '>': addRarrow(cshape, offset); letterOffset=4;
					break;
			case '<': addLarrow(cshape, offset); letterOffset=4;
					break;
			default: letterOffset=3;// nothing to do, (eg. it's a space)
					break;
			}

			offset = offset + btVector3(letterOffset,0,0);
		}
		Yoffset = Yoffset - btVector3(0,6,0);

	}
	return cshape;
}


//*****************************     A     ******************************************
//*****************************    A A    ******************************************
//*****************************   A   A   ******************************************
//*****************************  AAAAAAA  ******************************************
//***************************** A       A ******************************************
//*****************************A         A******************************************
void LetterDrawer::addA(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* a[3];

	a[0] = new btCapsuleShape(0.5,5.5);
	transform.setOrigin(offset + btVector3(0.3,2.4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.25,0.5,0)));
	cshape->addChildShape(transform,a[0]);

	a[1] = new btCapsuleShape(0.5,5.5);
	transform.setOrigin(offset + btVector3(2.7,2.4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.25,0.5,0)));
	cshape->addChildShape(transform,a[1]);

	a[2] = new btCapsuleShape(0.5,3);
	transform.setOrigin(offset + btVector3(1.5,1.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,a[2]);
}

void LetterDrawer::addB(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* p_1[8];

	p_1[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0,2.5,0));
	cshape->addChildShape(transform,p_1[0]);

	p_1[1] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,p_1[1]);

	p_1[2] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1,3,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,p_1[2]);

	p_1[3] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.4,2,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[3]);

	p_1[4] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.4,1,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[4]);

	p_1[5] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,p_1[5]);

	p_1[6] = new btCapsuleShape(0.5,1);
	transform.setOrigin(offset + btVector3(2.4,3.6,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[6]);

	p_1[7] = new btCapsuleShape(0.5,1);
	transform.setOrigin(offset + btVector3(2.4,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[7]);
}

void LetterDrawer::addC(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* c[5];

	c[0] = new btCapsuleShape(0.5,2.5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,c[0]);

	c[1] = new btCapsuleShape(0.5,1);
	transform.setOrigin(offset + btVector3(1,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,c[1]);

	c[2] = new btCapsuleShape(0.5,1);
	transform.setOrigin(offset + btVector3(1,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,c[2]);

	c[3] = new btCapsuleShape(0.5,1.7);
	transform.setOrigin(offset + btVector3(2.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,c[3]);

	c[4] = new btCapsuleShape(0.5,1.4);
	transform.setOrigin(offset + btVector3(2.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,c[4]);
}

void LetterDrawer::addD(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* g[6];

	g[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,g[0]);

	g[1] = new btCapsuleShape(0.5,2.5);
	transform.setOrigin(offset + btVector3(3.5,2.5,0));
	cshape->addChildShape(transform,g[1]);

	g[2] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[2]);

	g[3] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[3]);

	g[4] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(3,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[4]);

	g[5] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(3,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[5]);

}

void LetterDrawer::addE(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* e[4];

	e[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0,2.5,0));
	cshape->addChildShape(transform,e[0]);

	e[1] = new btCapsuleShape(0.5,2);
	transform.setOrigin(offset + btVector3(1.5,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,e[1]);

	e[2] = new btCapsuleShape(0.5,2);
	transform.setOrigin(offset + btVector3(1.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,e[2]);

	e[3] = new btCapsuleShape(0.5,2);
	transform.setOrigin(offset + btVector3(1.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,e[3]);
}

void LetterDrawer::addF(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* e[3];

	e[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0,2.5,0));
	cshape->addChildShape(transform,e[0]);

	e[1] = new btCapsuleShape(0.5,2);
	transform.setOrigin(offset + btVector3(1.5,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,e[1]);

	e[2] = new btCapsuleShape(0.5,2);
	transform.setOrigin(offset + btVector3(1.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,e[2]);
}

void LetterDrawer::addG(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* g[7];

	g[0] = new btCapsuleShape(0.5,2.5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,g[0]);

	g[1] = new btCapsuleShape(0.5,2);
	transform.setOrigin(offset + btVector3(3.5,1.1,0));
	cshape->addChildShape(transform,g[1]);

	g[2] = new btCapsuleShape(0.5,1);
	transform.setOrigin(offset + btVector3(1,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[2]);

	g[3] = new btCapsuleShape(0.5,1);
	transform.setOrigin(offset + btVector3(1,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[3]);

	g[4] = new btCapsuleShape(0.5,1.7);
	transform.setOrigin(offset + btVector3(2.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[4]);

	g[5] = new btCapsuleShape(0.5,1.4);
	transform.setOrigin(offset + btVector3(2.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[5]);

	g[6] = new btCapsuleShape(0.5,0.7);
	transform.setOrigin(offset + btVector3(3,2.3,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[6]);
}

void LetterDrawer::addH(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* h[3];

	h[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,h[0]);

	h[1] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(4,2.5,0));
	cshape->addChildShape(transform,h[1]);

	h[2] = new btCapsuleShape(0.5,3);
	transform.setOrigin(offset + btVector3(2.1,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,h[2]);
}

void LetterDrawer::addI(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* i[1];

	i[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(1,2.5,0));
	cshape->addChildShape(transform,i[0]);

}

void LetterDrawer::addJ(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* g[7];

	g[0] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(3,3,0));
	cshape->addChildShape(transform,g[0]);

	g[2] = new btCapsuleShape(0.5,1);
	transform.setOrigin(offset + btVector3(2.5,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[2]);

	g[5] = new btCapsuleShape(0.5,1.4);
	transform.setOrigin(offset + btVector3(1,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[5]);
}

void LetterDrawer::addK(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* p_1[3];

	p_1[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0,2.5,0));
	cshape->addChildShape(transform,p_1[0]);

	p_1[1] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(1.8,3.6,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,p_1[1]);

	p_1[2] = new btCapsuleShape(0.5,3.5);
	transform.setOrigin(offset + btVector3(2.2,1.3,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,p_1[2]);
}

//***************************** L         ******************************************
//***************************** L         ******************************************
//***************************** L         ******************************************
//***************************** L         ******************************************
//***************************** L         ******************************************
//***************************** LLLLLLL   ******************************************
void LetterDrawer::addL(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* l[2];

	l[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0,2.5,0));
	cshape->addChildShape(transform,l[0]);

	l[1] = new btCapsuleShape(0.5,3);
	transform.setOrigin(offset + btVector3(1.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,l[1]);
}

void LetterDrawer::addM(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* m[4];

	m[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,m[0]);

	m[1] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(4,2.5,0));
	cshape->addChildShape(transform,m[1]);

	m[2] = new btCapsuleShape(0.5,2.2);
	transform.setOrigin(offset + btVector3(1.5,4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.4,0.5,0)));
	cshape->addChildShape(transform,m[2]);

	m[3] = new btCapsuleShape(0.5,2.2);
	transform.setOrigin(offset + btVector3(3.25,4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.4,0.5,0)));
	cshape->addChildShape(transform,m[3]);
}

void LetterDrawer::addN(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* m[3];

	m[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,m[0]);

	m[1] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(4.1,2.5,0));
	cshape->addChildShape(transform,m[1]);

	m[2] = new btCapsuleShape(0.5,5.8);
	transform.setOrigin(offset + btVector3(2.3,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.4,0.5,0)));
	cshape->addChildShape(transform,m[2]);
}

void LetterDrawer::addO(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* g[8];

	g[0] = new btCapsuleShape(0.5,2.5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,g[0]);

	g[1] = new btCapsuleShape(0.5,2.5);
	transform.setOrigin(offset + btVector3(4.5,2.5,0));
	cshape->addChildShape(transform,g[1]);

	g[2] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(1,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[2]);

	g[3] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(1,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[3]);

	g[4] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[4]);

	g[5] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[5]);

	g[6] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(4,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[6]);

	g[7] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(4,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[7]);
}

void LetterDrawer::addP(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* p_1[5];

	p_1[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0,2.5,0));
	cshape->addChildShape(transform,p_1[0]);

	p_1[1] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1,2,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,p_1[1]);

	p_1[2] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,p_1[2]);

	p_1[3] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.4,4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[3]);

	p_1[4] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.4,3,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[4]);
}

void LetterDrawer::addQ(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* g[9];

	g[0] = new btCapsuleShape(0.5,2.5);
	transform.setOrigin(offset + btVector3(0.5,2.5,0));
	cshape->addChildShape(transform,g[0]);

	g[1] = new btCapsuleShape(0.5,2.5);
	transform.setOrigin(offset + btVector3(4.5,2.5,0));
	cshape->addChildShape(transform,g[1]);

	g[2] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(1,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[2]);

	g[3] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(1,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[3]);

	g[4] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[4]);

	g[5] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[5]);

	g[6] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(4,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[6]);

	g[7] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(4,4.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[7]);

	g[7] = new btCapsuleShape(0.5,1.4);
	transform.setOrigin(offset + btVector3(4,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[7]);
}

void LetterDrawer::addR(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* p_1[6];

	p_1[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0,2.5,0));
	cshape->addChildShape(transform,p_1[0]);

	p_1[1] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1,2,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,p_1[1]);

	p_1[2] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(1,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,p_1[2]);

	p_1[3] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.4,4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[3]);

	p_1[4] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.4,3,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.3,0.5,0)));
	cshape->addChildShape(transform,p_1[4]);

	p_1[5] = new btCapsuleShape(0.5,2.3);
	transform.setOrigin(offset + btVector3(2.4,1.1,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.2,0.5,0)));
	cshape->addChildShape(transform,p_1[5]);
}

void LetterDrawer::addS(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* g[7];

	g[0] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(2,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.8,0.5,0)));
	cshape->addChildShape(transform,g[0]);

	g[1] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(0.7,4.2,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.8,0)));
	cshape->addChildShape(transform,g[1]);

	g[2] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(3.4,0.7,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.8,0)));
	cshape->addChildShape(transform,g[2]);

	g[3] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[3]);

	g[4] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.2,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[4]);

	g[5] = new btCapsuleShape(0.5,1.3);
	transform.setOrigin(offset + btVector3(3.4,4.6,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.8,0.5,0)));
	cshape->addChildShape(transform,g[5]);

	g[6] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(0.7,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.8,0.5,0)));
	cshape->addChildShape(transform,g[6]);
}

void LetterDrawer::addT(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* y_1[2];

	y_1[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(2,2.5,0));
	cshape->addChildShape(transform,y_1[0]);

	y_1[1] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(2,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,y_1[1]);
}

void LetterDrawer::addU(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* g[5];

	g[0] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(0.5,3,0));
	cshape->addChildShape(transform,g[0]);

	g[1] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(4.5,3,0));
	cshape->addChildShape(transform,g[1]);

	g[2] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(1,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,g[2]);

	g[3] = new btCapsuleShape(0.5,1.5);
	transform.setOrigin(offset + btVector3(2.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,g[3]);

	g[4] = new btCapsuleShape(0.5,1.2);
	transform.setOrigin(offset + btVector3(4,0.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,g[4]);
}

void LetterDrawer::addV(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* a[2];

	a[0] = new btCapsuleShape(0.5,5.5);
	transform.setOrigin(offset + btVector3(0.3,2.4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.25,0.5,0)));
	cshape->addChildShape(transform,a[0]);

	a[1] = new btCapsuleShape(0.5,5.5);
	transform.setOrigin(offset + btVector3(2.7,2.4,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.25,0.5,0)));
	cshape->addChildShape(transform,a[1]);

}

void LetterDrawer::addW(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* m[4];

	m[0] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(0.3,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.1,0.5,0)));
	cshape->addChildShape(transform,m[0]);

	m[1] = new btCapsuleShape(0.5,5);
	transform.setOrigin(offset + btVector3(4.4,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.1,0.5,0)));
	cshape->addChildShape(transform,m[1]);

	m[2] = new btCapsuleShape(0.5,2.2);
	transform.setOrigin(offset + btVector3(1.5,1,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.4,0.5,0)));
	cshape->addChildShape(transform,m[2]);

	m[3] = new btCapsuleShape(0.5,2.2);
	transform.setOrigin(offset + btVector3(3.25,1,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.4,0.5,0)));
	cshape->addChildShape(transform,m[3]);
}

void LetterDrawer::addX(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* y_1[2];

	y_1[1] = new btCapsuleShape(0.5,5.5);
	transform.setOrigin(offset + btVector3(2,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.3,0.5,0)));
	cshape->addChildShape(transform,y_1[1]);

	y_1[2] = new btCapsuleShape(0.5,5.5);
	transform.setOrigin(offset + btVector3(2,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.3,0.5,0)));
	cshape->addChildShape(transform,y_1[2]);
}

void LetterDrawer::addY(btCompoundShape *cshape, btVector3 offset)
{
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* y_1[3];

	y_1[0] = new btCapsuleShape(0.5,3);
	transform.setOrigin(offset + btVector3(2.5,1.5,0));
	cshape->addChildShape(transform,y_1[0]);

	y_1[1] = new btCapsuleShape(0.5,2.7);
	transform.setOrigin(offset + btVector3(1.6,3.7,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.5,0)));
	cshape->addChildShape(transform,y_1[1]);

	y_1[2] = new btCapsuleShape(0.5,2.7);
	transform.setOrigin(offset + btVector3(3.4,3.7,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.5,0)));
	cshape->addChildShape(transform,y_1[2]);
}

void LetterDrawer::addZ(btCompoundShape* cshape, btVector3 offset){
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* shape[3];

	shape[0] = new btCapsuleShape(0.5,3.3);
	transform.setOrigin(offset + btVector3(2.5,5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,shape[0]);

	shape[1] = new btCapsuleShape(0.5,3.3);
	transform.setOrigin(offset + btVector3(2.5,0,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	cshape->addChildShape(transform,shape[1]);

	shape[2] = new btCapsuleShape(0.5,6);
	transform.setOrigin(offset + btVector3(2.5,2.5,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.4,0.5,0)));
	cshape->addChildShape(transform,shape[2]);

}

void LetterDrawer::addRarrow(btCompoundShape* cshape, btVector3 offset){
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* shape[3];

	shape[0] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(0,2,0));
	cshape->addChildShape(transform,shape[0]);

	shape[1] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(1.7,3,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.3,0)));
	cshape->addChildShape(transform,shape[1]);

	shape[2] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(1.7,1,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.3,0)));
	cshape->addChildShape(transform,shape[2]);

}

void LetterDrawer::addLarrow(btCompoundShape* cshape, btVector3 offset){
	btTransform	transform;
	transform.setIdentity();
	btCollisionShape* shape[3];

	shape[0] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(4,2,0));
	cshape->addChildShape(transform,shape[0]);

	shape[1] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(2.3,3,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(0.5,0.3,0)));
	cshape->addChildShape(transform,shape[1]);

	shape[2] = new btCapsuleShape(0.5,4);
	transform.setOrigin(offset + btVector3(2.3,1,0));
	transform.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-0.5,0.3,0)));
	cshape->addChildShape(transform,shape[2]);

}

btQuaternion* LetterDrawer::calculateRotation(btVector3* axis, btVector3* targetAxis) {
	//printf("axis: %f x %f x %f \n", axis->getX(),axis->getY(),axis->getZ());
	//printf("targetAxis: %f x %f x %f \n", targetAxis->getX(),targetAxis->getY(),targetAxis->getZ());
	btVector3 cross = targetAxis->cross(*axis);
	btScalar rotation_angle = targetAxis->angle(*axis);
	return new btQuaternion(cross, -rotation_angle);
}

