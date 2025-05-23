/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "claves_rpc.h"

bool_t
xdr_Coord_sensor (XDR *xdrs, Coord_sensor *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->x))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->y))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_set_value_peticion (XDR *xdrs, set_value_peticion *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_int (xdrs, &objp->key))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->value1, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->N_value2))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->V_value2, 32,
		sizeof (double), (xdrproc_t) xdr_double))
		 return FALSE;
	 if (!xdr_Coord_sensor (xdrs, &objp->value3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_get_value_respuesta (XDR *xdrs, get_value_respuesta *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_int (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->value1, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->N_value2))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->V_value2, 32,
		sizeof (double), (xdrproc_t) xdr_double))
		 return FALSE;
	 if (!xdr_Coord_sensor (xdrs, &objp->value3))
		 return FALSE;
	return TRUE;
}
