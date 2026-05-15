
	segment code

;-----------------------------------------------------------------------	
; Generic callback function for updating biquad coefficient.	
;-----------------------------------------------------------------------	
updateBiquadCallback LFUNC R1:R0=FunctionPointer32:l, R4=dsp:i, R5=proc:i, r6=ptr:i, r7=band:i

	push	dp1

	xpush	r7
	xpush	r6
	xpush	r5
	xpush	r4
		
	stw	r1, dp1h
	xrcall r0
	
	pop	dp1
	
	ret
	ENDFUNC

;-----------------------------------------------------------------------	
; dsp NRPN callback function 
;-----------------------------------------------------------------------	
dspNrpnHandlerCallback XFUNC FunctionPointer32:l, nrpn:i, dspId:i, processId:i, value:l, format:i

	push	dp1

	lxs	r7, format
	xpush	r7

	dlxs	dr3, value
	dxpush dr3
	
	lxs	r7, processId
	xpush	r7
	
	lxs	r7, dspId
	xpush	r7
	
	lxs	r7, nrpn
	xpush	r7
	
	dlxs	dr0, FunctionPointer32
	stw	r0, dp1h
	xrcall r1
	
	pop	dp1
	
	xret	$szparam
	ENDFUNC	
			
;-----------------------------------------------------------------------	
; swap DWORD  
;-----------------------------------------------------------------------			
swapDword LFUNC R1:R0=dword_data:l
	swap	dr0
	ret
	ENDFUNC
