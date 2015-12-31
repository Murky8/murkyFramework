#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

namespace murkyFramework {    
    inline mat43 makeRotationMatrix_from4c(vec4 v)
    {
        // create rotation matrix from a unit vector (v.xyz) and rotation scalar (v.w)
        // rotates clockwise along rotation axis (left handed coordsys).
        f32 xx, xy, xz, yy, yz, zz;
        f32 cw, sw;
        f32 cwxy, cwxz, cwyz;
        f32 swx, swy, swz;
        f32 cwxx, cwyy, cwzz;
        mat43 m{unit};

        cw = cosf(v.w);	// inf 1
        sw = sinf(v.w);	//	inf w

        xx = v.x*v.x;
        yy = v.y*v.y;
        zz = v.z*v.z;  //1-yy-xx

        xy = v.x*v.y;
        yz = v.y*v.z;
        xz = v.z*v.x;

        swx = sw*v.x;
        swy = sw*v.y;
        swz = sw*v.z;

        cwxy = cw*xy;
        cwxz = cw*xz;
        cwyz = cw*yz;

        cwxx = cw*xx;
        cwyy = cw*yy;
        cwzz = cw*zz;

        m.v[0][0] = xx + cw - cwxx;	
        m.v[0][1] = xy + -cwxy - swz;		
        m.v[0][2] = xz + -cwxz + swy;

        m.v[1][0] = xy + -cwxy + swz;
        m.v[1][1] = yy + cw - cwyy;
        m.v[1][2] = yz + -cwyz - swx;

        m.v[2][0] = xz + -cwxz - swy;
        m.v[2][1] = yz + -cwyz + swx;
        m.v[2][2] = zz + cw - cwzz;

        return m;
    }

    inline mat43 makeRotationMatrix_from3c(vec4 v)
    {
        // separate vector into , unit, length
        f32 len;
        vec4 dir;
        mat43 m;
        
        if (v.split_3c(dir, len))
        {
            m = makeRotationMatrix_from4c(vec4(dir.x, dir.y, dir.z, len));
        }
        else
        {
            m = mat43(unit);                    
        }
        return	m;
    }
}//namespace murkyFramework 
