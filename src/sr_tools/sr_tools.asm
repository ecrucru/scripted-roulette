; ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;
;  Scripted Roulette - version 0.2
;  Copyright (C) 2015-2016, http://scripted-roulette.sourceforge.net
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License along
;  with this program; if not, write to the Free Software Foundation, Inc.,
;  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
;
; ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;
;  The free download version of the Borland C++ Compiler version 5.5
;  doesn't include Turbo Assembler (TASM32). Therefore it is not possible
;  to compile the assembly code shipped with Scripted Roulette. The code
;  is then moved to a DLL which applies only if you link with BCC55.
;
;  The following file requires MASM32 to be compiled. Do use the BAT file.
;  The file should be stored on the same drive than MASM32.
;
;  If you use this file to compile an OBJ file, BCC will not recognize it.
;  Moreover it seems that the OBJ file cannot be converted with COFF2OMF.
;  That's why it is not proposed.
;
; ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

      .586
      .model flat, stdcall
      option casemap :none 

      includelib \masm32\lib\user32.lib

; ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
.CODE

    LibMain proc pInstance:DWORD, pReason:DWORD, pUnused:DWORD 
        RET
    LibMain endp

    bcc_randomize proc
        RDTSC
        ROL     EDX, 5
        ROR     EAX, 3
        XOR     EAX, EDX
        RET
    bcc_randomize endp

    bcc_getcomputerclock proc
        RDTSC
        ROL     EDX, 6
        NOT     EDX
        ROR     EAX, 4
        XCHG    DH,  AL
        IMUL    EAX, EDX
        XOR     EAX, EDX
        ROR     EAX, 3
        RET
    bcc_getcomputerclock endp

end LibMain
