/*************************************************
 * Velocity profiles for annular flow
 * 
 * Created: 2013-12-04
 * Author: Yaoyu Hu <huyaoyu@sjtu.edu.cn>
 *************************************************/

#include "udf.h"
#include <stdio.h>
#include <errno.h>

#define M_PI2 6.283185

/* the eccentricity */
#define E (0.5e-3)

/* the rotating speed of the rotor */
#define W 188.5

static real wirlSpeed = W;

#define R1 0.290
#define R2 0.295
#define gpmToMCubic (3.785411784e-3)

/* 50 degree */
#define DENSITY 994.69952

#define GPM 0.0

static real axialVelocity = GPM*gpmToMCubic/60.0 / (M_PI*(R2*R2-R1*R1));
static real C = R2-R1;
static real LogLawExp = 1.0/7;

static real U0 = 0.36*(W*R1);
static real UT = W*R1;
static real VelocityBound[2] = {0.0,100.0};

static real getVecAngle2(real* vec1, real* vec2)
{
	real dot = vec1[0]*vec2[0] + vec1[1]*vec2[1];
	real mag = sqrt(vec1[0]*vec1[0] + vec1[1]*vec1[1]) * sqrt(vec2[0]*vec2[0] + vec2[1]*vec2[1]);

	real temp = dot/mag;

	/* check the division result */
	if (temp > 1.0)
	{
		temp = 1.0;
	}
	else if (temp < -1.0)
	{
		temp = -1.0;
	}

	return acos(temp);
}

/*
* cle - clearance
* ecc - eccentricity
* r1 - radius of rotor
* lle - log law exponet
* theta - the angle of rotor center
* u0 - center line velocity
* rc - rotor center
* pos - position of the current face centroid
* Ut - tangential velocity
* UAngle - the angle of the tangential velocity
*/
static void putU(real cle, real ecc, real r1, real lle, real theta, real u0, const real* rc, const real* pos, real* Ut, real* UAngle)
{
	/* local variables */
	real vec0[2], vec1[2];	/* the temperary vectors */
	real r           = 0;	/* the radius of a face centroid */
	real phi         = 0;	/* the angel between the two vectors */
	real alpha		 = 0;	/* the angel of the current face */
	real localH      = 0;	/* the clearance at the specified angel */
	real delta       = 0;	/* the delta position of log low */
	real y           = 0;	/* the y of log law */
	real pow71       = 0;	/* the temporary pow value */
	real u0_2	 = 2*u0;/* the temporary 2*u0 value */

	/* velocity angle */
	real tempAngle;

	/* angle of the face centroid */
	alpha = atan(pos[0]/pos[2]);

	if (pos[2] >= 0)
	{
		alpha -= M_PI/2;
	}
	else
	{
		alpha = alpha - M_PI/2 + M_PI;
	}

	/* two vectors */
	vec0[0] = pos[0] - rc[0];
	vec0[1] = pos[2] - rc[1];

	vec1[0] = 0 - rc[0];
	vec1[1] = 0 - rc[1];

	/* get the angle phi */
	errno = 0;
	phi = getVecAngle2(vec0,vec1);

	if (errno != 0)
	{
		Message("errno neq 0. %f %d\n", phi, errno);
	}

	/* get the local H */
	localH = cle + ecc * cos(phi);

	/* get the raduis refer to the rotor center */
	r = sqrt(vec0[0]*vec0[0] + vec0[1]*vec0[1]);
	delta = localH / 2;
	y = r - r1;

	if (y <= 0)
	{
		/* if the face centroid is too close to the rotor */
		*Ut = UT;
	}
	/*else if (y >= cle || y > localH)*/
	else if (y > localH)
	{
		/* if the face centroid is too close to the stator */
		*Ut = 0;
	}
	else
	{
		/* the face centroid is in between the rotor and stator */
		if (y <= delta )
		{
			pow71 = pow(y/delta, lle);
			*Ut = u0_2 - u0 * pow71 + (1-pow71)*(UT-u0_2);
		}
		else
		{
			*Ut = u0 * pow((localH-y)/delta,lle);
		}
	}

	if (alpha >= theta)
	{
		tempAngle = theta + (M_PI-phi) + M_PI/2;
	}
	else
	{
		tempAngle = theta - (M_PI-phi) + M_PI/2;
	}

	/* change to standard angles */
	tempAngle = fmod(tempAngle,M_PI2);

	if (tempAngle >= M_PI)
	{
		tempAngle = tempAngle - M_PI2;
	}
	else if (tempAngle < -1 * M_PI)
	{
		tempAngle = tempAngle + M_PI2;
	}

	*UAngle = tempAngle;
}

static FILE* fopen_UDF(const char* fn, const char* mode)
{
	FILE* fp = NULL;
#if RP_NODE
	if (I_AM_NODE_ZERO_P)
	{
		if ((fp = fopen(fn, mode))==NULL)
			Message("\n Warning: Unable to open %s for writing\n",fn);
		else
			Message("\nNode 0: File %s opend.\n",fn);	
	}
	else
	{
		Message("Node%2d: In fopen_UDF.\n",myid);		
	}
#endif
#if !RP_NODE
	if ((fp = fopen(fn, mode))==NULL)
		Message("\n Warning: Unable to open %s for writing\n",fn);
	else
		Message("\nFile %s opend.\n",fn);
#endif

  return fp;
}

static void fclose_UDF(FILE* fp)
{
#if RP_NODE
	if (I_AM_NODE_ZERO_P)
	{
		fclose(fp);
		Message("Node 0: Close the file.\n");	
	}
#endif
#if !RP_NODE
	fclose(fp);
#endif
}

/* ====== The axis is the y-axis! ======= */

DEFINE_PROFILE(pre_swirl_velocity_u,t,idx)
{
	/* local variables */
	real pos[ND_ND];		/* the position of a face centroid */
	real rotorCenter[2];	/* the position of the rotor center */
	real theta       = 0;	/* the angel of the rotor center */
	real UAngle      = 0;	/* the angel of the tagential velocity from x-axis */
	real currentTime = CURRENT_TIME;
	real Ut          = 0;	/* the imposed velocity along the tagential direction */

	face_t f;

	FILE* fp = NULL;
	real tempVelocity;

	int badValueFound = 0;

	/* parallel consideration */
#if PARALLEL
	int size = 0;
	real* arrayPos0 = NULL;
	real* arrayPos1 = NULL;
	real* arrayPos2 = NULL;
	real* arrayCurrentTime = NULL;
	real* arrayTheta = NULL;
	real* arrayRotorCenter0 = NULL;
	real* arrayRotorCenter1 = NULL;
	real* arrayUt = NULL;
	real* arrayUAngle = NULL;
	real* arrayErrno = NULL;
	int pe = -1;
	int entryCount = 0;
	int i;
#endif

	/* print currentTime for testing */
#if PARALLEL
	/*Message("Node%2d: On index %d, current time is %f\n", myid, idx, currentTime);*/
	/*Message("Node%2d: hostid is %d\n", myid, node_host);*/
#else
	Message("On index %d, current time is %f\n", idx, currentTime);
#endif

	/* current position of the rotor center */
	theta = fmod(currentTime * wirlSpeed, 2*M_PI);
	if (theta > M_PI)
	{
		theta -= 2*M_PI;
	}

	rotorCenter[0] =      E * cos(theta);
	rotorCenter[1] = -1 * E * sin(theta);


	begin_f_loop(f,t)
	{
		if (PRINCIPAL_FACE_P(f,t))
		{
			F_CENTROID(pos,f,t);

			putU(C, E, R1, LogLawExp, theta, U0, rotorCenter, pos, &Ut, &UAngle);

			/* apply velocity profile */
			tempVelocity = Ut * cos(UAngle);
			
			F_PROFILE(f,t,idx) = tempVelocity;	
			
			if (Ut < VelocityBound[0] || Ut > VelocityBound[1])
			{
				if (badValueFound == 0)
				{
					/* loop for every face */
					fp = fopen_UDF("velocity_u.dat","a");

#if RP_NODE
					size  = THREAD_N_ELEMENTS_INT(t);
					arrayPos0 = (real *)malloc(size * sizeof(real));
					arrayPos1 = (real *)malloc(size * sizeof(real));
					arrayPos2 = (real *)malloc(size * sizeof(real));
					arrayCurrentTime = (real *)malloc(size * sizeof(real));
					arrayTheta = (real *)malloc(size * sizeof(real));
					arrayRotorCenter0 = (real *)malloc(size * sizeof(real));
					arrayRotorCenter1 = (real *)malloc(size * sizeof(real));
					arrayUt = (real *)malloc(size * sizeof(real));
					arrayUAngle = (real *)malloc(size * sizeof(real));
					arrayErrno = (real *)malloc(size * sizeof(real));
#endif
					badValueFound = 1;
				}
#if !PARALLEL
				fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n", pos[0], pos[1], pos[2], currentTime, theta, rotorCenter[0], rotorCenter[1], Ut, UAngle, errno);
#endif
#if RP_NODE
				arrayPos0[entryCount] = pos[0];
				arrayPos1[entryCount] = pos[1];
				arrayPos2[entryCount] = pos[2];
				arrayCurrentTime[entryCount] = currentTime;
				arrayTheta[entryCount] = theta;
				arrayRotorCenter0[entryCount] = rotorCenter[0];
				arrayRotorCenter1[entryCount] = rotorCenter[1];
				arrayUt[entryCount] = Ut;
				arrayUAngle[entryCount] = UAngle;
				arrayErrno[entryCount] = (real)errno;

				entryCount++;
#endif
			}
		}
	}
	end_f_loop(f,t)

#if RP_NODE
	if (badValueFound)
	{
		if (!(I_AM_NODE_ZERO_P))
		{
			/* if not Node 0, send messages to Node 0 */
			pe = node_zero;
			PRF_CSEND_INT(pe, &entryCount, 1, myid);
			PRF_CSEND_REAL(pe, arrayPos0, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayPos1, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayPos2, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayCurrentTime, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayTheta, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayRotorCenter0, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayRotorCenter1, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayUt, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayUAngle, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayErrno, entryCount, myid);
		}
		else
		{
			/* Node 0 accessed the file */
			for (i=0; i<entryCount; i++)
		    {
		    	fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n",
		    		arrayPos0[i], arrayPos1[i], arrayPos2[i], 
		    		arrayCurrentTime[i], 
		    		arrayTheta[i], 
		    		arrayRotorCenter0[i], arrayRotorCenter1[i], 
		    		arrayUt[i], arrayUAngle[i], 
		    		arrayErrno[i]);
		    }
		}

		free(arrayErrno);
		free(arrayUAngle);
		free(arrayUt);
		free(arrayRotorCenter1);
		free(arrayRotorCenter0);
		free(arrayTheta);
		free(arrayCurrentTime);
		free(arrayPos2);
		free(arrayPos1);
		free(arrayPos0);
	} /* badValueFound */
	else
	{
		if (!(I_AM_NODE_ZERO_P))
		{
			PRF_CSEND_INT(node_zero, &entryCount, 1, myid);
		}
	} /* badValueFound */
	

	/* node_0 now collect data sent by other compute nodes */
	/*  and sends it straight on to the host */

	if (I_AM_NODE_ZERO_P)
	{
		compute_node_loop_not_zero (pe)
	    {
			PRF_CRECV_INT(pe, &size, 1, pe);
			if (size != 0)
			{
				arrayPos0 = (real *)malloc(size * sizeof(real));
				arrayPos1 = (real *)malloc(size * sizeof(real));
				arrayPos2 = (real *)malloc(size * sizeof(real));
				arrayCurrentTime = (real *)malloc(size * sizeof(real));
				arrayTheta = (real *)malloc(size * sizeof(real));
				arrayRotorCenter0 = (real *)malloc(size * sizeof(real));
				arrayRotorCenter1 = (real *)malloc(size * sizeof(real));
				arrayUt = (real *)malloc(size * sizeof(real));
				arrayUAngle = (real *)malloc(size * sizeof(real));
				arrayErrno = (real *)malloc(size * sizeof(real));

				PRF_CRECV_REAL(pe, arrayPos0, size, pe);
				PRF_CRECV_REAL(pe, arrayPos1, size, pe);
				PRF_CRECV_REAL(pe, arrayPos2, size, pe);
				PRF_CRECV_REAL(pe, arrayCurrentTime, size, pe);
				PRF_CRECV_REAL(pe, arrayTheta, size, pe);
				PRF_CRECV_REAL(pe, arrayRotorCenter0, size, pe);
				PRF_CRECV_REAL(pe, arrayRotorCenter1, size, pe);
				PRF_CRECV_REAL(pe, arrayUt, size, pe);
				PRF_CRECV_REAL(pe, arrayUAngle, size, pe);
				PRF_CRECV_REAL(pe, arrayErrno, size, pe);

				/* acess to the file */
				for (i=0; i<size; i++)
			    {
			    	fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n",
			    		arrayPos0[i], arrayPos1[i], arrayPos2[i], 
			    		arrayCurrentTime[i], 
			    		arrayTheta[i], 
			    		arrayRotorCenter0[i], arrayRotorCenter1[i], 
			    		arrayUt[i], arrayUAngle[i], 
			    		arrayErrno[i]);
			    }

				free(arrayErrno);
				free(arrayUAngle);
				free(arrayUt);
				free(arrayRotorCenter1);
				free(arrayRotorCenter0);
				free(arrayTheta);
				free(arrayCurrentTime);
				free(arrayPos2);
				free(arrayPos1);
				free(arrayPos0);
			} /* size != 0 */
	    } /* node loop */
	} /* node zero */
#endif /* rp_node */

	if (badValueFound)
	{
		fclose_UDF(fp);		
	}
}

DEFINE_PROFILE(pre_swirl_velocity_w,t,idx)
{
	/* local variables */
	real pos[ND_ND];		/* the position of a face centroid */
	real rotorCenter[2];	/* the position of the rotor center */
	real theta       = 0;	/* the angel of the rotor center */
	real UAngle      = 0;	/* the angel of the tagential velocity from x-axis */
	real currentTime = CURRENT_TIME;
	real Ut          = 0;	/* the imposed velocity along the tagential direction */

	face_t f;

	FILE* fp = NULL;
	real tempVelocity;

	int badValueFound = 0;

	/* parallel consideration */
#if PARALLEL
	int size = 0;
	real* arrayPos0 = NULL;
	real* arrayPos1 = NULL;
	real* arrayPos2 = NULL;
	real* arrayCurrentTime = NULL;
	real* arrayTheta = NULL;
	real* arrayRotorCenter0 = NULL;
	real* arrayRotorCenter1 = NULL;
	real* arrayUt = NULL;
	real* arrayUAngle = NULL;
	real* arrayErrno = NULL;
	int pe = -1;
	int entryCount = 0;
	int i;
#endif

	/* print currentTime for testing */
#if PARALLEL
	/*Message("Node%2d: On index %d, current time is %f\n", myid, idx, currentTime);*/
	/*Message("Node%2d: hostid is %d\n", myid, node_host);*/
#else
	Message("On index %d, current time is %f\n", idx, currentTime);
#endif

	/* current position of the rotor center */
	theta = fmod(currentTime * wirlSpeed, 2*M_PI);
	if (theta > M_PI)
	{
		theta -= 2*M_PI;
	}

	rotorCenter[0] =      E * cos(theta);
	rotorCenter[1] = -1 * E * sin(theta);


	begin_f_loop(f,t)
	{
		if (PRINCIPAL_FACE_P(f,t))
		{
			F_CENTROID(pos,f,t);

			putU(C, E, R1, LogLawExp, theta, U0, rotorCenter, pos, &Ut, &UAngle);

			/* apply velocity profile */
			tempVelocity = -1 * Ut * sin(UAngle);
			
			F_PROFILE(f,t,idx) = tempVelocity;	
			
			if (Ut < VelocityBound[0] || Ut > VelocityBound[1])
			{
				if (badValueFound == 0)
				{
					/* loop for every face */
					fp = fopen_UDF("velocity_w.dat","a");

#if RP_NODE
					size  = THREAD_N_ELEMENTS_INT(t);
					arrayPos0 = (real *)malloc(size * sizeof(real));
					arrayPos1 = (real *)malloc(size * sizeof(real));
					arrayPos2 = (real *)malloc(size * sizeof(real));
					arrayCurrentTime = (real *)malloc(size * sizeof(real));
					arrayTheta = (real *)malloc(size * sizeof(real));
					arrayRotorCenter0 = (real *)malloc(size * sizeof(real));
					arrayRotorCenter1 = (real *)malloc(size * sizeof(real));
					arrayUt = (real *)malloc(size * sizeof(real));
					arrayUAngle = (real *)malloc(size * sizeof(real));
					arrayErrno = (real *)malloc(size * sizeof(real));
#endif
					badValueFound = 1;
				}
#if !PARALLEL
				fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n", pos[0], pos[1], pos[2], currentTime, theta, rotorCenter[0], rotorCenter[1], Ut, UAngle, errno);
#endif
#if RP_NODE
				arrayPos0[entryCount] = pos[0];
				arrayPos1[entryCount] = pos[1];
				arrayPos2[entryCount] = pos[2];
				arrayCurrentTime[entryCount] = currentTime;
				arrayTheta[entryCount] = theta;
				arrayRotorCenter0[entryCount] = rotorCenter[0];
				arrayRotorCenter1[entryCount] = rotorCenter[1];
				arrayUt[entryCount] = Ut;
				arrayUAngle[entryCount] = UAngle;
				arrayErrno[entryCount] = (real)errno;

				entryCount++;
#endif
			}
		}
	}
	end_f_loop(f,t)

#if RP_NODE
	if (badValueFound)
	{
		if (!(I_AM_NODE_ZERO_P))
		{
			/* if not Node 0, send messages to Node 0 */
			pe = node_zero;
			PRF_CSEND_INT(pe, &entryCount, 1, myid);
			PRF_CSEND_REAL(pe, arrayPos0, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayPos1, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayPos2, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayCurrentTime, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayTheta, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayRotorCenter0, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayRotorCenter1, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayUt, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayUAngle, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayErrno, entryCount, myid);
		}
		else
		{
			/* Node 0 accessed the file */
			for (i=0; i<entryCount; i++)
		    {
		    	fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n",
		    		arrayPos0[i], arrayPos1[i], arrayPos2[i], 
		    		arrayCurrentTime[i], 
		    		arrayTheta[i], 
		    		arrayRotorCenter0[i], arrayRotorCenter1[i], 
		    		arrayUt[i], arrayUAngle[i], 
		    		arrayErrno[i]);
		    }
		}

		free(arrayErrno);
		free(arrayUAngle);
		free(arrayUt);
		free(arrayRotorCenter1);
		free(arrayRotorCenter0);
		free(arrayTheta);
		free(arrayCurrentTime);
		free(arrayPos2);
		free(arrayPos1);
		free(arrayPos0);
	} /* badValueFound */
	else
	{
		if (!(I_AM_NODE_ZERO_P))
		{
			PRF_CSEND_INT(node_zero, &entryCount, 1, myid);
		}
	} /* badValueFound */
	

	/* node_0 now collect data sent by other compute nodes */
	/*  and sends it straight on to the host */

	if (I_AM_NODE_ZERO_P)
	{
		compute_node_loop_not_zero (pe)
	    {
			PRF_CRECV_INT(pe, &size, 1, pe);
			if (size != 0)
			{
				arrayPos0 = (real *)malloc(size * sizeof(real));
				arrayPos1 = (real *)malloc(size * sizeof(real));
				arrayPos2 = (real *)malloc(size * sizeof(real));
				arrayCurrentTime = (real *)malloc(size * sizeof(real));
				arrayTheta = (real *)malloc(size * sizeof(real));
				arrayRotorCenter0 = (real *)malloc(size * sizeof(real));
				arrayRotorCenter1 = (real *)malloc(size * sizeof(real));
				arrayUt = (real *)malloc(size * sizeof(real));
				arrayUAngle = (real *)malloc(size * sizeof(real));
				arrayErrno = (real *)malloc(size * sizeof(real));

				PRF_CRECV_REAL(pe, arrayPos0, size, pe);
				PRF_CRECV_REAL(pe, arrayPos1, size, pe);
				PRF_CRECV_REAL(pe, arrayPos2, size, pe);
				PRF_CRECV_REAL(pe, arrayCurrentTime, size, pe);
				PRF_CRECV_REAL(pe, arrayTheta, size, pe);
				PRF_CRECV_REAL(pe, arrayRotorCenter0, size, pe);
				PRF_CRECV_REAL(pe, arrayRotorCenter1, size, pe);
				PRF_CRECV_REAL(pe, arrayUt, size, pe);
				PRF_CRECV_REAL(pe, arrayUAngle, size, pe);
				PRF_CRECV_REAL(pe, arrayErrno, size, pe);

				/* acess to the file */
				for (i=0; i<size; i++)
			    {
			    	fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n",
			    		arrayPos0[i], arrayPos1[i], arrayPos2[i], 
			    		arrayCurrentTime[i], 
			    		arrayTheta[i], 
			    		arrayRotorCenter0[i], arrayRotorCenter1[i], 
			    		arrayUt[i], arrayUAngle[i], 
			    		arrayErrno[i]);
			    }

				free(arrayErrno);
				free(arrayUAngle);
				free(arrayUt);
				free(arrayRotorCenter1);
				free(arrayRotorCenter0);
				free(arrayTheta);
				free(arrayCurrentTime);
				free(arrayPos2);
				free(arrayPos1);
				free(arrayPos0);
			} /* size != 0 */
	    } /* node loop */
	} /* node zero */
#endif /* rp_node */

	if (badValueFound)
	{
		fclose_UDF(fp);		
	}
}

DEFINE_PROFILE(pre_swirl_velocity_v,t,idx)
{
	face_t f;

	begin_f_loop(f,t)
	{
		if (PRINCIPAL_FACE_P(f,t))
		{
			F_PROFILE(f,t,idx) = axialVelocity;	
		}
	}
	end_f_loop(f,t)
}

DEFINE_PROFILE(pre_swirl_velocity_t,t,idx)
{
	/* local variables */
	real pos[ND_ND];		/* the position of a face centroid */
	real rotorCenter[2];	/* the position of the rotor center */
	real theta       = 0;	/* the angel of the rotor center */
	real UAngle      = 0;	/* the angel of the tagential velocity from x-axis */
	real currentTime = CURRENT_TIME;
	real Ut          = 0;	/* the imposed velocity along the tagential direction */

	face_t f;

	FILE* fp = NULL;

	int badValueFound = 0;

	/* parallel consideration */
#if PARALLEL
	int size = 0;
	real* arrayPos0 = NULL;
	real* arrayPos1 = NULL;
	real* arrayPos2 = NULL;
	real* arrayCurrentTime = NULL;
	real* arrayTheta = NULL;
	real* arrayRotorCenter0 = NULL;
	real* arrayRotorCenter1 = NULL;
	real* arrayUt = NULL;
	real* arrayUAngle = NULL;
	real* arrayErrno = NULL;
	int pe = -1;
	int entryCount = 0;
	int i;
#endif

	/* print currentTime for testing */
#if PARALLEL
	/*Message("Node%2d: On index %d, current time is %f\n", myid, idx, currentTime);*/
	/*Message("Node%2d: hostid is %d\n", myid, node_host);*/
#else
	Message("On index %d, current time is %f\n", idx, currentTime);
#endif

	/* current position of the rotor center */
	theta = fmod(currentTime * wirlSpeed, 2*M_PI);
	if (theta > M_PI)
	{
		theta -= 2*M_PI;
	}

	rotorCenter[0] =      E * cos(theta);
	rotorCenter[1] = -1 * E * sin(theta);


	begin_f_loop(f,t)
	{
		if (PRINCIPAL_FACE_P(f,t))
		{
			F_CENTROID(pos,f,t);

			putU(C, E, R1, LogLawExp, theta, U0, rotorCenter, pos, &Ut, &UAngle);

			/* apply velocity profile */
			F_PROFILE(f,t,idx) = Ut;	
			
			if (Ut < VelocityBound[0] || Ut > VelocityBound[1])
			{
				if (badValueFound == 0)
				{
					/* loop for every face */
					fp = fopen_UDF("velocity_t.dat","a");

#if RP_NODE
					size  = THREAD_N_ELEMENTS_INT(t);
					arrayPos0 = (real *)malloc(size * sizeof(real));
					arrayPos1 = (real *)malloc(size * sizeof(real));
					arrayPos2 = (real *)malloc(size * sizeof(real));
					arrayCurrentTime = (real *)malloc(size * sizeof(real));
					arrayTheta = (real *)malloc(size * sizeof(real));
					arrayRotorCenter0 = (real *)malloc(size * sizeof(real));
					arrayRotorCenter1 = (real *)malloc(size * sizeof(real));
					arrayUt = (real *)malloc(size * sizeof(real));
					arrayUAngle = (real *)malloc(size * sizeof(real));
					arrayErrno = (real *)malloc(size * sizeof(real));
#endif
					badValueFound = 1;
				}
#if !PARALLEL
				fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n", pos[0], pos[1], pos[2], currentTime, theta, rotorCenter[0], rotorCenter[1], Ut, UAngle, errno);
#endif
#if RP_NODE
				arrayPos0[entryCount] = pos[0];
				arrayPos1[entryCount] = pos[1];
				arrayPos2[entryCount] = pos[2];
				arrayCurrentTime[entryCount] = currentTime;
				arrayTheta[entryCount] = theta;
				arrayRotorCenter0[entryCount] = rotorCenter[0];
				arrayRotorCenter1[entryCount] = rotorCenter[1];
				arrayUt[entryCount] = Ut;
				arrayUAngle[entryCount] = UAngle;
				arrayErrno[entryCount] = (real)errno;

				entryCount++;
#endif
			}
		}
	}
	end_f_loop(f,t)

#if RP_NODE
	if (badValueFound)
	{
		if (!(I_AM_NODE_ZERO_P))
		{
			/* if not Node 0, send messages to Node 0 */
			pe = node_zero;
			PRF_CSEND_INT(pe, &entryCount, 1, myid);
			PRF_CSEND_REAL(pe, arrayPos0, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayPos1, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayPos2, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayCurrentTime, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayTheta, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayRotorCenter0, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayRotorCenter1, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayUt, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayUAngle, entryCount, myid);
			PRF_CSEND_REAL(pe, arrayErrno, entryCount, myid);
		}
		else
		{
			/* Node 0 accessed the file */
			for (i=0; i<entryCount; i++)
		    {
		    	fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n",
		    		arrayPos0[i], arrayPos1[i], arrayPos2[i], 
		    		arrayCurrentTime[i], 
		    		arrayTheta[i], 
		    		arrayRotorCenter0[i], arrayRotorCenter1[i], 
		    		arrayUt[i], arrayUAngle[i], 
		    		arrayErrno[i]);
		    }
		}

		free(arrayErrno);
		free(arrayUAngle);
		free(arrayUt);
		free(arrayRotorCenter1);
		free(arrayRotorCenter0);
		free(arrayTheta);
		free(arrayCurrentTime);
		free(arrayPos2);
		free(arrayPos1);
		free(arrayPos0);
	} /* badValueFound */
	else
	{
		if (!(I_AM_NODE_ZERO_P))
		{
			PRF_CSEND_INT(node_zero, &entryCount, 1, myid);
		}
	} /* badValueFound */
	

	/* node_0 now collect data sent by other compute nodes */
	/*  and sends it straight on to the host */

	if (I_AM_NODE_ZERO_P)
	{
		compute_node_loop_not_zero (pe)
	    {
			PRF_CRECV_INT(pe, &size, 1, pe);
			if (size != 0)
			{
				arrayPos0 = (real *)malloc(size * sizeof(real));
				arrayPos1 = (real *)malloc(size * sizeof(real));
				arrayPos2 = (real *)malloc(size * sizeof(real));
				arrayCurrentTime = (real *)malloc(size * sizeof(real));
				arrayTheta = (real *)malloc(size * sizeof(real));
				arrayRotorCenter0 = (real *)malloc(size * sizeof(real));
				arrayRotorCenter1 = (real *)malloc(size * sizeof(real));
				arrayUt = (real *)malloc(size * sizeof(real));
				arrayUAngle = (real *)malloc(size * sizeof(real));
				arrayErrno = (real *)malloc(size * sizeof(real));

				PRF_CRECV_REAL(pe, arrayPos0, size, pe);
				PRF_CRECV_REAL(pe, arrayPos1, size, pe);
				PRF_CRECV_REAL(pe, arrayPos2, size, pe);
				PRF_CRECV_REAL(pe, arrayCurrentTime, size, pe);
				PRF_CRECV_REAL(pe, arrayTheta, size, pe);
				PRF_CRECV_REAL(pe, arrayRotorCenter0, size, pe);
				PRF_CRECV_REAL(pe, arrayRotorCenter1, size, pe);
				PRF_CRECV_REAL(pe, arrayUt, size, pe);
				PRF_CRECV_REAL(pe, arrayUAngle, size, pe);
				PRF_CRECV_REAL(pe, arrayErrno, size, pe);

				/* acess to the file */
				for (i=0; i<size; i++)
			    {
			    	fprintf(fp, "%f %f %f %f %f %f %f %f %f %d\n",
			    		arrayPos0[i], arrayPos1[i], arrayPos2[i], 
			    		arrayCurrentTime[i], 
			    		arrayTheta[i], 
			    		arrayRotorCenter0[i], arrayRotorCenter1[i], 
			    		arrayUt[i], arrayUAngle[i], 
			    		arrayErrno[i]);
			    }

				free(arrayErrno);
				free(arrayUAngle);
				free(arrayUt);
				free(arrayRotorCenter1);
				free(arrayRotorCenter0);
				free(arrayTheta);
				free(arrayCurrentTime);
				free(arrayPos2);
				free(arrayPos1);
				free(arrayPos0);
			} /* size != 0 */
	    } /* node loop */
	} /* node zero */
#endif /* rp_node */

	if (badValueFound)
	{
		fclose_UDF(fp);		
	}
}

/*
 * The calculation is done in xz plane!
 *
 * rc - rotor center, length = 2
 * bp - boundary point, length = 2
 * ut - the tangential velocity on the surface of the rotor with respect to the rotor center, m/s
 * wirlAV - wirl angular velocity, rad/s
 * u - velocity, output, length = 2
 */
static void putRotU(const real* rc, const real* bp, real ut, real wirlAV, real* u)
{
	real translationVelocity[2] = {0,0};
	real bpr = 0; /* the magnitude of vector bp */
	real bpa = 0; /* the angle of vector bp */
	real translation = 0; /* the translation velocity magnitude */
	real bpVec[2] = {0,0}; /* the relative vector of the boundary point to the center of the rotor */
	real bpva = 0; /* the angle of the relative vector bpVec */
	
	bpr = sqrt(*(bp) * *(bp) + *(bp+1) * *(bp+1));
	bpa = atan(-1 * *(bp+1) / *(bp));
	
	if (*(bp) < 0)
	{
		if (bpa < 0)
		{
			bpa += M_PI;
		}
		else if (bpa > 0)
		{
			bpa -= M_PI;
		}
	}

	bpa += 0.5*M_PI;
	
	/* translational velocity */
	translation = bpr * wirlAV;
	
	translationVelocity[0] =    translation * cos(bpa);
	translationVelocity[1] = -1*translation * sin(bpa);
	
	/* relative vector */
	bpVec[0] = bp[0] - rc[0];
	bpVec[1] = bp[1] - rc[1];
	
	bpva = atan(-1*bpVec[1]/bpVec[0]);
	if (bpVec[0] < 0)
	{
		if (bpva < 0)
		{
			bpva += M_PI;
		}
		else if (bpva > 0)
		{
			bpva -= M_PI;
		}
	}
	
	/* rotate the vector */
	bpva += M_PI/2;
	
	*(u)   =    ut * cos(bpva) + translationVelocity[0];
	*(u+1) = -1*ut * sin(bpva) + translationVelocity[1];
}

DEFINE_PROFILE(wirl_v_u,t,idx)
{
	/* local variables */
	real pos[ND_ND];				/* the position of a face centroid */
	real pos2[2] = {0,0};			/* the position of a face centroid, x and z */
	real rotorCenter[2] = {0,0};	/* the position of the rotor center */
	real theta       = 0;	/* the angel of the rotor center */
	real currentTime = CURRENT_TIME;
	real UVec[2]     = {0,0};	/* the imposed velocity */

	face_t f;

	/* current position of the rotor center */
	theta = fmod(currentTime * wirlSpeed, 2*M_PI);
	if (theta > M_PI)
	{
		theta -= 2*M_PI;
	}

	rotorCenter[0] =      E * cos(theta);
	rotorCenter[1] = -1 * E * sin(theta);


	begin_f_loop(f,t)
	{
		if (PRINCIPAL_FACE_P(f,t))
		{
			F_CENTROID(pos,f,t);
			
			pos2[0] = pos[0];
			pos2[1] = pos[2];

			putRotU(rotorCenter, pos2, UT, wirlSpeed, UVec);

			/* apply velocity profile */
			F_PROFILE(f,t,idx) = UVec[0];	
		}
	}
	end_f_loop(f,t)
}

DEFINE_PROFILE(wirl_v_w,t,idx)
{
	/* local variables */
	real pos[ND_ND];				/* the position of a face centroid */
	real pos2[2] = {0,0};			/* the position of a face centroid, x and z */
	real rotorCenter[2] = {0,0};	/* the position of the rotor center */
	real theta       = 0;	/* the angel of the rotor center */
	real currentTime = CURRENT_TIME;
	real UVec[2]     = {0,0};	/* the imposed velocity */

	face_t f;

	/* current position of the rotor center */
	theta = fmod(currentTime * wirlSpeed, 2*M_PI);
	if (theta > M_PI)
	{
		theta -= 2*M_PI;
	}

	rotorCenter[0] =      E * cos(theta);
	rotorCenter[1] = -1 * E * sin(theta);


	begin_f_loop(f,t)
	{
		if (PRINCIPAL_FACE_P(f,t))
		{
			F_CENTROID(pos,f,t);
			
			pos2[0] = pos[0];
			pos2[1] = pos[2];

			putRotU(rotorCenter, pos2, UT, wirlSpeed, UVec);

			/* apply velocity profile */
			F_PROFILE(f,t,idx) = UVec[1];	
		}
	}
	end_f_loop(f,t)
}
