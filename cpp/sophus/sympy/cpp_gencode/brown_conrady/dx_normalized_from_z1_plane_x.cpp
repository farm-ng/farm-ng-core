Scalar const c0 = pow(a, 2);
Scalar const c1 = pow(b, 2);
Scalar const c2 = c0 + c1;
Scalar const c3 = pow(c2, 2);
Scalar const c4 = pow(c2, 3);
Scalar const c5 = c2*d[5] + c3*d[6] + c4*d[7] + 1.0;
Scalar const c6 = pow(c5, 2);
Scalar const c7 = 1.0/c6;
Scalar const c8 = a*d[3];
Scalar const c9 = 2.0*d[2];
Scalar const c10 = 2*c2;
Scalar const c11 = 3*c3;
Scalar const c12 = c2*d[0];
Scalar const c13 = c3*d[1];
Scalar const c14 = c4*d[4];
Scalar const c15 = 2.0*(c10*d[6] + c11*d[7] + d[5])*(c12 + c13 + c14 + 1.0);
Scalar const c16 = 2.0*c10*d[1] + 2.0*c11*d[4] + 2.0*d[0];
Scalar const c17 = 1.0*c12 + 1.0*c13 + 1.0*c14 + 1.0;
Scalar const c18 = b*d[3];
Scalar const c19 = a*b;
Scalar const c20 = -c15*c19 + c16*c19*c5;
result[0] = c7*(-c0*c15 + c5*(c0*c16 + c17) + c6*(b*c9 + 6.0*c8));
result[1] = c7*(c20 + c6*(a*c9 + 2*c18));
result[2] = c7*(c20 + c6*(2*a*d[2] + 2.0*c18));
result[3] = c7*(-c1*c15 + c5*(c1*c16 + c17) + c6*(6.0*b*d[2] + 2.0*c8));
