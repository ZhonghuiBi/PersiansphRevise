/***********************************************************************************
* PersianSPH - A C++ library to simulate Mechanical Systems (solids, fluids        *
*             and soils) using Smoothed Particle Hydrodynamics method              *
* Copyright (C) 2013 Maziar Gholami Korzani and Sergio Galindo-Torres              *
*                                                                                  *
* This file is part of PersianSPH                                                  *
*                                                                                  *
* This is free software; you can redistribute it and/or modify it under the        *
* terms of the GNU General Public License as published by the Free Software        *
* Foundation; either version 3 of the License, or (at your option) any later       *
* version.                                                                         *
*                                                                                  *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY  *
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A  *
* PARTICULAR PURPOSE. See the GNU General Public License for more details.         *
*                                                                                  *
* You should have received a copy of the GNU General Public License along with     *
* PersianSPH; if not, see <http://www.gnu.org/licenses/>                           *
************************************************************************************/

#include "Particle.h"

namespace SPH {

inline Particle::Particle(int Tag, Vec3_t const & x0, Vec3_t const & v0, double Mass0, double Density0, double h0,bool Fixed)
{
	ct = 0;
	a = 0.0;
    x = x0;
    n = 0.0;
    n0 = 0.0;
    k = 0.0;
    k2 = 0.0;

    Cs		= 0.0;
    P0		= 0.0;
    PresEq	= 0;
    Alpha	= 0.0;
    Beta	= 0.0;

    va = 0.0;
    vb = 0.0;
    NSv = 0.0;
    FSINSv = 0.0;
    v = v0;
    VXSPH = 0.0;
    TI		= 0.0;
    TIn		= 4.0;
    TIInitDist  = 0.0;

    Densitya = 0.0;
    Densityb = 0.0;
    Density = Density0;
    RefDensity = Density0;

    Mass = Mass0;
    FPMassC = 1.0;
    IsFree = !Fixed;
    h = h0;
    Pressure=0.0;
    FSIPressure=0.0;
    ID = Tag;
    CC[0]= CC[1] = CC[2] = 0;
    LL=0;
    ZWab = 0.0;
    SumDen = 0.0;
    dDensity=0.0;
    ShearRate = 0.0;
    MuRef = Mu = 0.0;
		VisM = 0;
    T0 = 0.0;
    m = 300.0;
    SumKernel = 0.0;
    FSISumKernel = 0.0;
    G = 0.0;
    K = 0.0;
    Material = 0;
    Fail = 0;
    c = 0.0;
    phi = 0.0;
    psi = 0.0;
    d =0.0;
    Sigmay = 0.0;
    NoSlip = false;
    Shepard = false;
    InOut = 0;
    FirstStep = true;
    V = Mass/RefDensity;
    RhoF = 0.0;
    IsSat = false;
    SatCheck = false;
    ShepardStep = 40;
    ShepardCounter = 0;
    S = 0.0;
    VarPorosity = false;
    SeepageType = 0;
    S = 0;
	LES = false;
	SBar = 0.0;
	CSmag = 0.17;



    set_to_zero(Strainb);
    set_to_zero(Strain);
    set_to_zero(Sigmab);
    set_to_zero(Sigma);
//    set_to_zero(FSISigma);
    set_to_zero(Sigmaa);
    set_to_zero(ShearStress);
    set_to_zero(ShearStressb);
    set_to_zero(TIR);
    set_to_zero(StrainRate);
    set_to_zero(RotationRate);
    omp_init_lock(&my_lock);

}

inline void Particle::Move(double dt, Vec3_t Domainsize, Vec3_t domainmax, Vec3_t domainmin, size_t Scheme, Mat3_t I)
{
	if (Scheme == 0)
		Move_MVerlet(I, dt);
	else
		Move_Leapfrog(I, dt);


	//Periodic BC particle position update
	if (Domainsize(0)>0.0)
	{
		(x(0)>(domainmax(0))) ? x(0) -= Domainsize(0) : x(0);
		(x(0)<(domainmin(0))) ? x(0) += Domainsize(0) : x(0);
	}
	if (Domainsize(1)>0.0)
	{
		(x(1)>(domainmax(1))) ? x(1) -= Domainsize(1) : x(1);
		(x(1)<(domainmin(1))) ? x(1) += Domainsize(1) : x(1);
	}
	if (Domainsize(2)>0.0)
	{
		(x(2)>(domainmax(2))) ? x(2) -= Domainsize(2) : x(2);
		(x(2)<(domainmin(2))) ? x(2) += Domainsize(2) : x(2);
	}

}

inline void Particle::Mat1(double dt)
{
	Pressure 	= EOS(PresEq, Cs, P0,Density, RefDensity);
	double temp	= (StrainRate(0,0)*StrainRate(0,0) + 2.0*StrainRate(0,1)*StrainRate(1,0) +
			2.0*StrainRate(0,2)*StrainRate(2,0) + StrainRate(1,1)*StrainRate(1,1) +
			2.0*StrainRate(1,2)*StrainRate(2,1) + StrainRate(2,2)*StrainRate(2,2));

	ShearRate	= sqrt(0.5*temp);
	SBar		= sqrt(2.0*temp);

	// LES model
	if (LES)
	{
		Mu	= MuRef + RefDensity*pow((CSmag*h),2.0)*SBar;
	}

	// Bingham viscosity calculation
	if (T0>0.0)
	{
		switch (VisM)
		{
			case 0:
			// Bingham
				if (ShearRate !=0.0)
					Mu = MuRef + T0*(1-exp(-m*ShearRate))/ShearRate;
				else
					Mu = MuRef + T0*m;
				break;
			case 1:
			// Cross
				Mu = (1000.0*MuRef + MuRef*MuRef*1000.0/T0*ShearRate)/(1+1000.0*MuRef/T0*ShearRate);
				break;
			default:
				std::cout << "Non-Newtonian Viscosity Type No is out of range. Please correct it and run again" << std::endl;
				std::cout << "0 => Bingham" << std::endl;
				std::cout << "1 => Cross" << std::endl;
				abort();
				break;
		}
	}
}


inline void Particle::Move_MVerlet (Mat3_t I, double dt)
{
	if (FirstStep)
	{
		ct = 30;
		FirstStep = false;
	}

	x += dt*(v+VXSPH) + 0.5*dt*dt*a;

	if (ct == 30)
	{
		if (Shepard && ShepardCounter == ShepardStep)
		{
			if (ZWab>0.6)
			{
				Densityb	= SumDen/ZWab;
//				Densityb	= Density;
				Density		= SumDen/ZWab;
			}
			else
			{
				Densityb	= Density;
				Density		+=dt*dDensity;
			}
		}
		else
		{
			Densityb		= Density;
			Density			+=dt*dDensity;
		}

		vb	= v;
		v	+=dt*a;
	}
	else
	{
		if (Shepard && ShepardCounter == ShepardStep)
		{
			if (ZWab>0.6)
			{
				Densityb	= SumDen/ZWab;
//				Densityb	= Density;
				Density		= SumDen/ZWab;
			}
			else
			{
				double dens	= Density;
				Density		= Densityb + 2.0*dt*dDensity;
				Densityb	= dens;
			}
		}
		else
		{
			double dens	= Density;
			Density		= Densityb + 2.0*dt*dDensity;
			Densityb	= dens;
		}

		Vec3_t temp;
		temp	= v;
		v		= vb + 2*dt*a;
		vb		= temp;
	}

	switch (Material)
    {case 1:
    	Mat1(dt);
		break;
    case 2:
    	Mat2MVerlet(dt);
    	break;
    case 3:
    	Mat3MVerlet(I,dt);
    	break;
   default:
	   	std::cout << "Material Type No is out of range. Please correct it and run again" << std::endl;
		std::cout << "1 => Fluid" << std::endl;
		std::cout << "2 => Solid" << std::endl;
		std::cout << "3 => Soil" << std::endl;
	    abort();
	    break;
    }
	if (ct == 30) ct = 0; else ct++;
	if (ShepardCounter == ShepardStep) ShepardCounter = 0; else ShepardCounter++;
}

inline void Particle::Mat2MVerlet(double dt)
{
	Pressure = EOS(PresEq, Cs, P0,Density, RefDensity);

	// Jaumann rate terms
	Mat3_t RotationRateT, Stress,SRT,RS;
	Trans(RotationRate,RotationRateT);
	Mult(ShearStress,RotationRateT,SRT);
	Mult(RotationRate,ShearStress,RS);

	// Elastic prediction step (ShearStress_e n+1)
	Stress			= ShearStress;
	if (ct == 30)
		ShearStress	= dt*(2.0*G*(StrainRate-1.0/3.0*(StrainRate(0,0)+StrainRate(1,1)+StrainRate(2,2))*OrthoSys::I)+SRT+RS) + ShearStress;
	else
		ShearStress	= 2.0*dt*(2.0*G*(StrainRate-1.0/3.0*(StrainRate(0,0)+StrainRate(1,1)+StrainRate(2,2))*OrthoSys::I)+SRT+RS) + ShearStressb;
	ShearStressb	= Stress;

	if (Fail == 1)
	{
		double J2	= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
						2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
						2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));
		//Scale back
		ShearStress	= std::min((Sigmay/sqrt(3.0*J2)),1.0)*ShearStress;
	}

	Sigma			= -Pressure * OrthoSys::I + ShearStress;

	Stress	= Strain;
	if (ct == 30)
		Strain	= dt*StrainRate + Strain;
	else
		Strain	= 2.0*dt*StrainRate + Strainb;
	Strainb	= Stress;


	if (Fail > 1)
	{
		std::cout<<"Undefined failure criteria for solids"<<std::endl;
		abort();
	}
}

inline void Particle::Mat3MVerlet(Mat3_t I, double dt)
{
	Mat3_t RotationRateT, Stress, SRT,RS;
	double I1,J2,alpha,kf,I1strain;

	// Jaumann rate terms
	Trans(RotationRate,RotationRateT);
	Mult(Sigma,RotationRateT,SRT);
	Mult(RotationRate,Sigma,RS);

	// Volumetric strain
	I1strain = StrainRate(0,0)+StrainRate(1,1)+StrainRate(2,2);

	// Elastic prediction step (Sigma_e n+1)
	Stress	= Sigma;
	if (ct == 30)
		Sigma	= dt*(I1strain*K*OrthoSys::I + 2.0*G*(StrainRate-1.0/3.0*I1strain*OrthoSys::I) + SRT + RS) + Sigma;
	else
		Sigma	= 2.0*dt*( I1strain*K*OrthoSys::I + 2.0*G*(StrainRate-1.0/3.0*I1strain*OrthoSys::I) + SRT + RS) + Sigmab;
	Sigmab	= Stress;

	if (Fail>1)
	{
		if (I(2,2)==0.0)
		{
			// Drucker-Prager failure criterion for plane strain
			alpha	= tan(phi) / sqrt(9.0+12.0*tan(phi)*tan(phi));
			kf		= 3.0 * c  / sqrt(9.0+12.0*tan(phi)*tan(phi));
		}
		else
		{
			// Drucker-Prager failure criterion for 3D
			alpha	= (2.0*  sin(phi)) / (sqrt(3.0)*(3.0-sin(phi)));
			kf		= (6.0*c*cos(phi)) / (sqrt(3.0)*(3.0-sin(phi)));
		}


		// Bring back stress to the apex of the failure criteria
		I1		= Sigma(0,0) + Sigma(1,1) + Sigma(2,2);
		if ((kf-alpha*I1)<0.0)
		{
			double Ratio;
			if (alpha == 0.0) Ratio =0.0; else Ratio = kf/alpha;
			Sigma(0,0) -= 1.0/3.0*(I1-Ratio);
			Sigma(1,1) -= 1.0/3.0*(I1-Ratio);
			Sigma(2,2) -= 1.0/3.0*(I1-Ratio);
			I1 			= Ratio;
		}

		// Shear stress based on the elastic assumption (S_e n+1)
		ShearStress = Sigma - 1.0/3.0* I1 *OrthoSys::I;
		J2 			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
						2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
						2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));


		// Check the elastic prediction step by the failure criteria
		if ((sqrt(J2)+alpha*I1-kf)>0.0)
		{
			// Shear stress based on the existing stress (S n)
			ShearStress = Stress - 1.0/3.0*(Stress(0,0)+Stress(1,1)+Stress(2,2))*OrthoSys::I;
			J2 			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
							2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
							2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));

			if (sqrt(J2)>0.0)
			{
				Mat3_t temp, Plastic;
				double sum,dLanda;

				// calculating the plastic term based on the existing shear stress and strain rate
				temp	= abab(ShearStress,StrainRate);
				sum		= temp(0,0)+temp(0,1)+temp(0,2)+temp(1,0)+temp(1,1)+temp(1,2)+temp(2,0)+temp(2,1)+temp(2,2);
				switch (Fail)
				{
				case 2:
					dLanda	= 1.0/(9.0*alpha*alpha*K+G)*( (3.0*alpha*K*I1strain) + (G/sqrt(J2))*sum );
					Plastic	= 3.0*alpha*K*I + G/sqrt(J2)*ShearStress;
					break;
				case 3:
					dLanda	= 1.0/(9.0*alpha*K*3.0*sin(psi)+G)*( (3.0*alpha*K*I1strain) + (G/sqrt(J2))*sum );
					Plastic	= 3.0*3.0*sin(psi)*K*I + G/sqrt(J2)*ShearStress;
					break;
				default:
					std::cout << "Failure Type No is out of range. Please correct it and run again" << std::endl;
					std::cout << "2 => Associated flow rule" << std::endl;
					std::cout << "3 => non-associated flow rule" << std::endl;
					abort();
					break;
				}
				// Apply the plastic term
				if (ct == 30)
					Sigma = Sigma -	dt*(dLanda*Plastic);
				else
					Sigma = Sigma -	2.0*dt*(dLanda*Plastic);
			}

			//Scale back
			I1			= Sigma(0,0) + Sigma(1,1) + Sigma(2,2);
			if ((kf-alpha*I1)<0.0)
			{
				double Ratio;
				if (alpha == 0.0) Ratio =0.0; else Ratio = kf/alpha;
				Sigma(0,0) -= 1.0/3.0*(I1-Ratio);
				Sigma(1,1) -= 1.0/3.0*(I1-Ratio);
				Sigma(2,2) -= 1.0/3.0*(I1-Ratio);
				I1 			= Ratio;
			}
			ShearStress	= Sigma - 1.0/3.0* I1 *OrthoSys::I;
			J2			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
							2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
							2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));

			if ((sqrt(J2)+alpha*I1-kf)>0.0 && sqrt(J2)>0.0) Sigma = I1/3.0*OrthoSys::I + (kf-alpha*I1)/sqrt(J2) * ShearStress;
		}
	}

	Stress	= Strain;
	if (ct == 30)
		Strain	= dt*StrainRate + Strain;
	else
		Strain	= 2.0*dt*StrainRate + Strainb;
	Strainb	= Stress;

	if (VarPorosity)
	{
		if (IsFree)
		{
			double ev = (Strain(0,0)+Strain(1,1)+Strain(2,2));
			n = (n0+ev)/(1.0+ev);
			switch(SeepageType)
			{
				case 0:
					break;
				case 1:
					k = n*n*n*d*d/(180.0*(1.0-n)*(1.0-n));
					break;
				case 2:
					k = n*n*n*d*d/(150.0*(1.0-n)*(1.0-n));
					k2= 1.75*(1.0-n)/(n*n*n*d);
					break;
				case 3:
					k = n*n*n*d*d/(150.0*(1.0-n)*(1.0-n));
					k2= 0.4/(n*n*d);
					break;
				default:
					std::cout << "Seepage Type No is out of range. Please correct it and run again" << std::endl;
					std::cout << "0 => Darcy's Law" << std::endl;
					std::cout << "1 => Darcy's Law & Kozeny???Carman Eq" << std::endl;
					std::cout << "2 => The Forchheimer Eq & Ergun Coeffs" << std::endl;
					std::cout << "3 => The Forchheimer Eq & Den Adel Coeffs" << std::endl;
					abort();
					break;
			}
		}
		else
			n = n0;
	}
	else
		n = n0;


}

inline void Particle::ScalebackMat3(size_t Dimension,size_t Scheme)
{
	double I1,J2,alpha,kf;

	if (Dimension==0.0)
	{
		// Drucker-Prager failure criterion for plane strain
		alpha	= tan(phi) / sqrt(9.0+12.0*tan(phi)*tan(phi));
		kf		= 3.0 * c  / sqrt(9.0+12.0*tan(phi)*tan(phi));
	}
	else
	{
		// Drucker-Prager failure criterion for 3D
		alpha	= (2.0*  sin(phi)) / (sqrt(3.0)*(3.0-sin(phi)));
		kf		= (6.0*c*cos(phi)) / (sqrt(3.0)*(3.0-sin(phi)));
	}

	// Bring back stressb to the apex of the failure criteria
	I1	= Sigmab(0,0) + Sigmab(1,1) + Sigmab(2,2);
	if ((kf-alpha*I1)<0.0)
	{
		double Ratio;
		if (alpha == 0.0) Ratio =0.0; else Ratio = kf/alpha;
		Sigmab(0,0) -= 1.0/3.0*(I1-Ratio);
		Sigmab(1,1) -= 1.0/3.0*(I1-Ratio);
		Sigmab(2,2) -= 1.0/3.0*(I1-Ratio);
		I1 	     = Ratio;
	}

	ShearStress	= Sigmab - 1.0/3.0* I1 *OrthoSys::I;
	J2			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
				2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
				2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));

	if ((sqrt(J2)+alpha*I1-kf)>0.0 && sqrt(J2)>0.0) Sigmab = I1/3.0*OrthoSys::I + (kf-alpha*I1)/sqrt(J2) * ShearStress;

	// Bring back stress to the apex of the failure criteria
	if (Scheme == 0)
	{
		I1	= Sigma(0,0) + Sigma(1,1) + Sigma(2,2);
		if ((kf-alpha*I1)<0.0)
		{
			double Ratio;
			if (alpha == 0.0) Ratio =0.0; else Ratio = kf/alpha;
			Sigma(0,0) -= 1.0/3.0*(I1-Ratio);
			Sigma(1,1) -= 1.0/3.0*(I1-Ratio);
			Sigma(2,2) -= 1.0/3.0*(I1-Ratio);
			I1 	    = Ratio;
		}

		ShearStress	= Sigma - 1.0/3.0* I1 *OrthoSys::I;
		J2			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
					2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
					2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));

		if ((sqrt(J2)+alpha*I1-kf)>0.0 && sqrt(J2)>0.0) Sigma = I1/3.0*OrthoSys::I + (kf-alpha*I1)/sqrt(J2) * ShearStress;
	}
	else
	{
		I1	= Sigmaa(0,0) + Sigmaa(1,1) + Sigmaa(2,2);
		if ((kf-alpha*I1)<0.0)
		{
			double Ratio;
			if (alpha == 0.0) Ratio =0.0; else Ratio = kf/alpha;
			Sigmaa(0,0) -= 1.0/3.0*(I1-Ratio);
			Sigmaa(1,1) -= 1.0/3.0*(I1-Ratio);
			Sigmaa(2,2) -= 1.0/3.0*(I1-Ratio);
			I1 			= Ratio;
		}
		ShearStress	= Sigmaa - 1.0/3.0* I1 *OrthoSys::I;
		J2			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
						2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
						2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));
		if ((sqrt(J2)+alpha*I1-kf)>0.0 && sqrt(J2)>0.0) Sigmaa = I1/3.0*OrthoSys::I + (kf-alpha*I1)/sqrt(J2) * ShearStress;
	}
}

inline void Particle::Move_Leapfrog(Mat3_t I, double dt)
{
	if (FirstStep)
	{
		Densitya = Density - dt/2.0*dDensity;
		va = v - dt/2.0*a;
	}
	Densityb = Densitya;
	Densitya += dt*dDensity;
	Density = (Densitya+Densityb)/2.0;
	vb = va;
	va += dt*a;
	v = (va + vb)/2.0;
	x += dt*va;

	switch (Material)
    {case 1:
    	Mat1(dt);
		break;
    case 2:
    	Mat2Leapfrog(dt);
    	break;
    case 3:
    	Mat3Leapfrog(I,dt);
    	break;
   default:
	   	std::cout << "Material Type No is out of range. Please correct it and run again" << std::endl;
		std::cout << "1 => Fluid" << std::endl;
		std::cout << "2 => Solid" << std::endl;
		std::cout << "3 => Soil" << std::endl;
	    abort();
	    break;
    }
	if (FirstStep) FirstStep = false;

}

inline void Particle::Mat2Leapfrog(double dt)
{
	Pressure = EOS(PresEq, Cs, P0,Density, RefDensity);

	// Jaumann rate terms
	Mat3_t RotationRateT,SRT,RS;
	Trans(RotationRate,RotationRateT);
	Mult(ShearStress,RotationRateT,SRT);
	Mult(RotationRate,ShearStress,RS);

	// Elastic prediction step (ShearStress_e n+1)
	if (FirstStep)
		ShearStressa	= -dt/2.0*(2.0*G*(StrainRate-1.0/3.0*(StrainRate(0,0)+StrainRate(1,1)+StrainRate(2,2))*OrthoSys::I)+SRT+RS) + ShearStress;

	ShearStressb	= ShearStressa;
	ShearStressa	= dt*(2.0*G*(StrainRate-1.0/3.0*(StrainRate(0,0)+StrainRate(1,1)+StrainRate(2,2))*OrthoSys::I)+SRT+RS) + ShearStressa;

	if (Fail == 1)
	{
		double J2	= 0.5*(ShearStressa(0,0)*ShearStressa(0,0) + 2.0*ShearStressa(0,1)*ShearStressa(1,0) +
						2.0*ShearStressa(0,2)*ShearStressa(2,0) + ShearStressa(1,1)*ShearStressa(1,1) +
						2.0*ShearStressa(1,2)*ShearStressa(2,1) + ShearStressa(2,2)*ShearStressa(2,2));
		//Scale back
		ShearStressa= std::min((Sigmay/sqrt(3.0*J2)),1.0)*ShearStressa;
	}
	ShearStress	= 1.0/2.0*(ShearStressa+ShearStressb);

	Sigma = -Pressure * OrthoSys::I + ShearStress;

	if (FirstStep)
		Straina	= -dt/2.0*StrainRate + Strain;
	Strainb	= Straina;
	Straina	= dt*StrainRate + Straina;
	Strain	= 1.0/2.0*(Straina+Strainb);


	if (Fail > 1)
	{
		std::cout<<"Undefined failure criteria for solids"<<std::endl;
		abort();
	}
}

inline void Particle::Mat3Leapfrog(Mat3_t I, double dt)
{
	Mat3_t RotationRateT, Stress, SRT,RS;
	double I1,J2,alpha,kf,I1strain;

	// Jaumann rate terms
	Trans(RotationRate,RotationRateT);
	Mult(Sigma,RotationRateT,SRT);
	Mult(RotationRate,Sigma,RS);

	// Volumetric strain
	I1strain = StrainRate(0,0)+StrainRate(1,1)+StrainRate(2,2);

	// Elastic prediction step (Sigma_e n+1)
	if (FirstStep)
		Sigmaa	= -dt/2.0*(I1strain*K*OrthoSys::I + 2.0*G*(StrainRate-1.0/3.0*I1strain*OrthoSys::I) + SRT + RS) + Sigma;

	Sigmab	= Sigmaa;
	Sigmaa	= dt*(I1strain*K*OrthoSys::I + 2.0*G*(StrainRate-1.0/3.0*I1strain*OrthoSys::I) + SRT + RS) + Sigmaa;

	if (Fail>1)
	{
		if (I(2,2)==0.0)
		{
			// Drucker-Prager failure criterion for plane strain
			alpha	= tan(phi) / sqrt(9.0+12.0*tan(phi)*tan(phi));
			kf		= 3.0 * c  / sqrt(9.0+12.0*tan(phi)*tan(phi));
		}
		else
		{
			// Drucker-Prager failure criterion for 3D
			alpha	= (2.0*  sin(phi)) / (sqrt(3.0)*(3.0-sin(phi)));
			kf		= (6.0*c*cos(phi)) / (sqrt(3.0)*(3.0-sin(phi)));
		}

		// Bring back stress to the apex of the failure criteria
		I1		= Sigmaa(0,0) + Sigmaa(1,1) + Sigmaa(2,2);
		if ((kf-alpha*I1)<0.0)
		{
			double Ratio;
			if (alpha == 0.0) Ratio =0.0; else Ratio = kf/alpha;
			Sigmaa(0,0) -= 1.0/3.0*(I1-Ratio);
			Sigmaa(1,1) -= 1.0/3.0*(I1-Ratio);
			Sigmaa(2,2) -= 1.0/3.0*(I1-Ratio);
			I1 			= Ratio;
		}

		// Shear stress based on the elastic assumption (S_e n+1)
		ShearStress = Sigmaa - 1.0/3.0* I1 *OrthoSys::I;
		J2 			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
						2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
						2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));


		// Check the elastic prediction step by the failure criteria
		if ((sqrt(J2)+alpha*I1-kf)>0.0)
		{
			// Shear stress based on the existing stress (S n)
			ShearStress = Sigma - 1.0/3.0*(Sigma(0,0)+Sigma(1,1)+Sigma(2,2))*OrthoSys::I;
			J2 			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
							2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
							2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));

			if (sqrt(J2)>0.0)
			{
				Mat3_t temp, Plastic;
				double sum,dLanda;

				// calculating the plastic term based on the existing shear stress and strain rate
				temp	= abab(ShearStress,StrainRate);
				sum		= temp(0,0)+temp(0,1)+temp(0,2)+temp(1,0)+temp(1,1)+temp(1,2)+temp(2,0)+temp(2,1)+temp(2,2);
				switch (Fail)
				{
				case 2:
					dLanda	= 1.0/(9.0*alpha*alpha*K+G)*( (3.0*alpha*K*I1strain) + (G/sqrt(J2))*sum );
					Plastic	= 3.0*alpha*K*I + G/sqrt(J2)*ShearStress;
					break;
				case 3:
					dLanda	= 1.0/(9.0*alpha*K*3.0*sin(psi)+G)*( (3.0*alpha*K*I1strain) + (G/sqrt(J2))*sum );
					Plastic	= 3.0*3.0*sin(psi)*K*I + G/sqrt(J2)*ShearStress;
					break;
				default:
					std::cout << "Failure Type No is out of range. Please correct it and run again" << std::endl;
					std::cout << "2 => Associated flow rule" << std::endl;
					std::cout << "3 => non-associated flow rule" << std::endl;
					abort();
					break;
				}
				Sigmaa = Sigmaa - dt*(dLanda*Plastic);
			}

			I1	= Sigmaa(0,0) + Sigmaa(1,1) + Sigmaa(2,2);
			if ((kf-alpha*I1)<0.0)
			{
				double Ratio;
				if (alpha == 0.0) Ratio =0.0; else Ratio = kf/alpha;
				Sigmaa(0,0) -= 1.0/3.0*(I1-Ratio);
				Sigmaa(1,1) -= 1.0/3.0*(I1-Ratio);
				Sigmaa(2,2) -= 1.0/3.0*(I1-Ratio);
				I1 			= Ratio;
			}
			ShearStress	= Sigmaa - 1.0/3.0* I1 *OrthoSys::I;
			J2			= 0.5*(ShearStress(0,0)*ShearStress(0,0) + 2.0*ShearStress(0,1)*ShearStress(1,0) +
							2.0*ShearStress(0,2)*ShearStress(2,0) + ShearStress(1,1)*ShearStress(1,1) +
							2.0*ShearStress(1,2)*ShearStress(2,1) + ShearStress(2,2)*ShearStress(2,2));
			if ((sqrt(J2)+alpha*I1-kf)>0.0 && sqrt(J2)>0.0) Sigmaa = I1/3.0*OrthoSys::I + (kf-alpha*I1)/sqrt(J2) * ShearStress;
		}
	}
	Sigma = 1.0/2.0*(Sigmaa+Sigmab);

	if (FirstStep)
		Straina	= -dt/2.0*StrainRate + Strain;
	Strainb	= Straina;
	Straina	= dt*StrainRate + Straina;
	Strain	= 1.0/2.0*(Straina+Strainb);

	if (VarPorosity)
	{
		if (IsFree)
		{
			double ev = (Strain(0,0)+Strain(1,1)+Strain(2,2));
			n = (n0+ev)/(1.0+ev);
			switch(SeepageType)
			{
				case 0:
					break;
				case 1:
					k = n*n*n*d*d/(180.0*(1.0-n)*(1.0-n));
					break;
				case 2:
					k = n*n*n*d*d/(150.0*(1.0-n)*(1.0-n));
					k2= 1.75*(1.0-n)/(n*n*n*d);
					break;
				case 3:
					k = n*n*n*d*d/(150.0*(1.0-n)*(1.0-n));
					k2= 0.4/(n*n*d);
					break;
				default:
					std::cout << "Seepage Type No is out of range. Please correct it and run again" << std::endl;
					std::cout << "0 => Darcy's Law" << std::endl;
					std::cout << "1 => Darcy's Law & Kozeny???Carman Eq" << std::endl;
					std::cout << "2 => The Forchheimer Eq & Ergun Coeffs" << std::endl;
					std::cout << "3 => The Forchheimer Eq & Den Adel Coeffs" << std::endl;
					abort();
					break;
			}
		}
		else
			n = n0;
	}
	else
		n = n0;


}

inline void Particle::translate(double dt, Vec3_t Domainsize, Vec3_t domainmax, Vec3_t domainmin)
{
	x = x + dt*v + 0.5*dt*dt*a;

	// Evolve velocity
	Vec3_t temp;
	temp = v;
	v = vb + 2*dt*a;
	vb = temp;

	//Periodic BC particle position update
	if (Domainsize(0)>0.0)
	{
		(x(0)>(domainmax(0))) ? x(0) -= Domainsize(0) : x(0);
		(x(0)<(domainmin(0))) ? x(0) += Domainsize(0) : x(0);
	}
	if (Domainsize(1)>0.0)
	{
		(x(1)>(domainmax(1))) ? x(1) -= Domainsize(1) : x(1);
		(x(1)<(domainmin(1))) ? x(1) += Domainsize(1) : x(1);
	}
	if (Domainsize(2)>0.0)
	{
		(x(2)>(domainmax(2))) ? x(2) -= Domainsize(2) : x(2);
		(x(2)<(domainmin(2))) ? x(2) += Domainsize(2) : x(2);
	}
}

}; // namespace SPH
