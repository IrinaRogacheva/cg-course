#pragma once
#include "CGLApplication.h"
#include "CChargedParticle.h"
#include <vector>
#include <optional>

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char* title, int width = 0, int height = 0, bool needDepth = true);
	~CMyApplication(void);
	size_t GetCountOfParticles() const;
	//const CChargedParticle& GetParticleAtIndex(size_t index) const;
	CChargedParticle& GetParticleAtIndex(size_t index);
protected:
	// Перегружаем необходимые виртуальные методы родительского класса
	virtual void OnInit();
	virtual void OnDisplay(void);
	virtual void OnReshape(int width, int height);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnKeyboard(unsigned char key, int x, int y);
private:
	std::optional<size_t> FindParticleAtPoint(Point point);
	std::optional<size_t> FindClosestParticle(size_t index);

	static void Callback(int value);

	std::vector<CChargedParticle> m_chargedParticles;

	static CMyApplication* m_pApp;
};
