/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "part_c.h"

bool_t
xdr_Path (XDR *xdrs, Path *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, 1000))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_parameters (XDR *xdrs, parameters *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->number1))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->number2))
		 return FALSE;
	 if (!xdr_Path (xdrs, &objp->path))
		 return FALSE;
	return TRUE;
}
