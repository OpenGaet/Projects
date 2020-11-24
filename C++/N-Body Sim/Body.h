// Object
class Body {

private:
	// Mass
	double mass = 0;
	// Position
	double xpos = 0;
	double ypos = 0;
	double zpos = 0;
	// Velocity
	double xvel = 0;
	double yvel = 0;
	double zvel = 0;
	// Acceleration
	double xacc = 0;
	double yacc = 0;
	double zacc = 0;
	// Gravitational Force
	double gravFx = 0;
	double gravFy = 0;

public:
	// Get Body Attributes
	double getMass() { return mass; }
	double getXpos() { return xpos; }
	double getYpos() { return ypos; }
	double getZpos() { return zpos; }
	double getXvel() { return xvel; }
	double getYvel() { return yvel; }
	double getZvel() { return zvel; }
	double getXacc() { return xacc; }
	double getYacc() { return yacc; }
	double getZacc() { return zacc; }
	double getXgrav() { return gravFx; }
	double getYgrav() { return gravFy; }

	// Set Body Attributes
	void setMass(double m) { mass = m; }
	void setXpos(double xp) { xpos = xp; }
	void setYpos(double yp) { ypos = yp; }
	void setZpos(double zp) { zpos = zp; }
	void setXvel(double xv) { xvel = xv; }
	void setYvel(double yv) { yvel = yv; }
	void setZvel(double zv) { zvel = zv; }
	void setXacc(double xa) { xacc = xa; }
	void setYacc(double ya) { yacc = ya; }
	void setZacc(double za) { zacc = za; }
	void setXgrav(double xg) { gravFx = xg; }
	void setYgrav(double yg) { gravFy = yg; }

	// Calc Attributes
	double calcDist(double x1, double y1, double x2, double y2);
	void calcGrav(Body*);
	void calcAccel();
	void calcVel();
	void calcPos();

	Body() {};
	Body(double mass, double xp, double yp, double zp, double xv, double yv, double zv);
};