/*
	gcc -O patch.c ofd_datalib.c -o patch
	./patch
*/


#include "ofd_datalib.h"
#include <math.h>
#include <stdio.h>

int main(void){
	const double freq = 2.5e9;
	const double lambda = 3e8 / freq;
	const double dt = 1.6e-3;           // Dielectric thickness
	const double A = 100e-3;            // Dielectric X-size
	const double B = 140e-3;			// Dielecrtic Y-size
	const double margin1 = 0.5*lambda;
	const double margin2 = 0.25*lambda;
	const double er = 4.3;
	const double tand = 0.001;
	const double eps0 = 8.854e-12;
	const double pi = 4 * atan(1);
    const double sigma = er * tand * (2 * pi * freq) * eps0;

    // ===Parameter===
    const double W = 36.858e-3;			// Patch width
    const double L = 28.564e-3;			// Patch length
    const double w_sl = 1e-3;         	// Slit width
    const double w_li = 3e-3;         	// Line width
    const double l_de = 10.8e-3;        // Line depth
    // ===============

	// initialize

	ofd_init();

	// title

	ofd_title("Patch Antenna");

	// mesh
    // x
    const double x0 = A/2;
    const double x1 = W/2;
    const double x2 = w_li/2 + w_sl;
    const double x3 = w_li/2;
	const int xmesh_dielectric = (x0-x1) / (lambda/100);
	const int xmesh_pec = (x1-x2) / (lambda/100);
    const int xmesh_slit = 2;
    const int xmesh_line = 4;
	ofd_xsection(8, -x0, -x1, -x2, -x3, x3, x2, x1, x0);
	ofd_xdivision(7, xmesh_dielectric, xmesh_pec, xmesh_slit, xmesh_line, xmesh_slit, xmesh_pec, xmesh_dielectric);
    
    // y
	const double y0 = B/2;
	const double y1 = L/2;
    const double y2 = y1 - l_de;
	const int ymesh_dielectric =  (y0 - y1) / (lambda/100);
	const int ymesh_slit = (l_de) / (lambda/100);
    const int ymesh_pec = (y1 + y2) / (lambda/100);
	ofd_ysection(5, -y0, -y1, -y2, y1, y0);
	ofd_ydivision(4,ymesh_dielectric, ymesh_slit, ymesh_pec, ymesh_dielectric);
    // z
	const double z0 = 0;
	const double z1 = z0 + dt;
	const double z2 = z1 + margin1;
	const int zmesh_dt = 3;
	const int zmesh_margin = (margin1) / (lambda/100);
	ofd_zsection(4, -2e-3, z0, z1, z2);
	ofd_zdivision(3, 2, zmesh_dt, zmesh_margin);


	// material
    ofd_material(er, sigma, 1, 0, "");

	// geometry

	ofd_geometry(2, 1, -x0, x0, -y0, y0, z0, z1);       // substrate

    ofd_geometry(1, 1, -x1, x1, -y1, y1, z1, z1);       
    ofd_geometry(2, 1, -x2, x2, -y1, -y2, z1, z1);
    ofd_geometry(1, 1, -x3, x3, -y0, -y2, z1, z1);      // Patch and Line

    ofd_geometry(1, 1, -x0, x0, -y0, y0, z0, -2.0e-3);	// ground




	// feed

	ofd_feed('Z', 0, -60e-3, 1.0e-3, 1, 0, 50);

	// point
	ofd_point('Z', 0, -20e-3, 1.0e-3, "+Y");

	// ABC

	// ofd_pml(5, 2, 1.0e-5);

	// frequency

	ofd_frequency1(2e-9, 3e-9, 200);
	ofd_frequency2(freq, freq, 0);

	// solver

	ofd_solver(30000, 1000, 1e-3);

	// iteration

	ofd_plotiter(1);

	// spara
	ofd_plotspara(1, 0, 0, 0);

	// far-1d

	ofd_plotfar1d('Y', 360, 0);

	// output options

	ofd_far1dstyle(0);
	ofd_far1dcomponent(1, 0, 0);
	ofd_far1ddb(1);

	// near-2d
	
	ofd_plotnear2d("E",'Z',0);
	ofd_near2ddim(1, 0);
	ofd_near2dframe(10);
	ofd_near2dscale(0, 1);
	ofd_near2dcontour(0);
	ofd_near2dobj(1);
	ofd_near2dnoinc();

	// window option
	ofd_window2d(750, 500, 15, 0);

	// output

	ofd_outdata("patch.ofd");

	return 0;
}
