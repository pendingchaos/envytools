#ifndef NVRM_OBJECT_XML
#define NVRM_OBJECT_XML

/* Autogenerated file, DO NOT EDIT manually!

This file was generated by the rules-ng-ng headergen tool in this git repository:
http://github.com/envytools/envytools/
git clone https://github.com/envytools/envytools.git

The rules-ng-ng source files this header was generated from are:
- /home/marcin/nv/envytools/rnndb/../docs/nvrm/rnndb/nvrm_object.xml (   8538 bytes, from 2014-09-11 09:55:42)
- /home/marcin/nv/envytools/rnndb/copyright.xml                      (   6452 bytes, from 2014-05-18 22:25:04)
- /home/marcin/nv/envytools/rnndb/nvchipsets.xml                     (   2759 bytes, from 2014-10-01 20:38:02)

Copyright (C) 2006-2014 by the following authors:
- Artur Huillet <arthur.huillet@free.fr> (ahuillet)
- Ben Skeggs (darktama, darktama_)
- B. R. <koala_br@users.sourceforge.net> (koala_br)
- Carlos Martin <carlosmn@users.sf.net> (carlosmn)
- Christoph Bumiller <e0425955@student.tuwien.ac.at> (calim, chrisbmr)
- Dawid Gajownik <gajownik@users.sf.net> (gajownik)
- Dmitry Baryshkov
- Dmitry Eremin-Solenikov <lumag@users.sf.net> (lumag)
- EdB <edb_@users.sf.net> (edb_)
- Erik Waling <erikwailing@users.sf.net> (erikwaling)
- Francisco Jerez <currojerez@riseup.net> (curro)
- imirkin <imirkin@users.sf.net> (imirkin)
- jb17bsome <jb17bsome@bellsouth.net> (jb17bsome)
- Jeremy Kolb <kjeremy@users.sf.net> (kjeremy)
- Laurent Carlier <lordheavym@gmail.com> (lordheavy)
- Luca Barbieri <luca@luca-barbieri.com> (lb, lb1)
- Maarten Maathuis <madman2003@gmail.com> (stillunknown)
- Marcin Kościelnicki <koriakin@0x04.net> (mwk, koriakin)
- Mark Carey <mark.carey@gmail.com> (careym)
- Matthieu Castet <matthieu.castet@parrot.com> (mat-c)
- nvidiaman <nvidiaman@users.sf.net> (nvidiaman)
- Patrice Mandin <patmandin@gmail.com> (pmandin, pmdata)
- Pekka Paalanen <pq@iki.fi> (pq, ppaalanen)
- Peter Popov <ironpeter@users.sf.net> (ironpeter)
- Richard Hughes <hughsient@users.sf.net> (hughsient)
- Rudi Cilibrasi <cilibrar@users.sf.net> (cilibrar)
- Serge Martin
- Simon Raffeiner
- Stephane Loeuillet <leroutier@users.sf.net> (leroutier)
- Stephane Marchesin <stephane.marchesin@gmail.com> (marcheu)
- sturmflut <sturmflut@users.sf.net> (sturmflut)
- Sylvain Munaut <tnt@246tNt.com>
- Victor Stinner <victor.stinner@haypocalc.com> (haypo)
- Wladmir van der Laan <laanwj@gmail.com> (miathan6)
- Younes Manton <younes.m@gmail.com> (ymanton)

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#define NVRM_CONTEXT_NEW					0x00000000
#define NVRM_CONTEXT						0x00000041
#define NVRM_DEVICE_0						0x00000080
#define NVRM_DEVICE_1						0x00000081
#define NVRM_DEVICE_2						0x00000082
#define NVRM_DEVICE_3						0x00000083
#define NVRM_DEVICE_4						0x00000084
#define NVRM_DEVICE_5						0x00000085
#define NVRM_DEVICE_6						0x00000086
#define NVRM_DEVICE_7						0x00000087
#define NVRM_SUBDEVICE_0					0x00002080
#define NVRM_SUBDEVICE_1					0x00002081
#define NVRM_SUBDEVICE_2					0x00002082
#define NVRM_SUBDEVICE_3					0x00002083
#define NVRM_TIMER						0x00000004
#define NVRM_MEMORY_SYSRAM					0x0000003e
#define NVRM_MEMORY_UNK003F					0x0000003f
#define NVRM_MEMORY_UNK0040					0x00000040
#define NVRM_MEMORY_VM						0x00000070
#define NVRM_MEMORY_UNK0071					0x00000071
#define NVRM_UNK0073						0x00000073
#define NVRM_UNK208F						0x0000208f
#define NVRM_EVENT						0x00000079
#define NVRM_PEEPHOLE_NV30					0x0000307e
#define NVRM_PEEPHOLE_GF100					0x00009068
#define NVRM_UNK83DE						0x000083de
#define NVRM_UNK85B6						0x000085b6
#define NVRM_UNK9096						0x00009096
#define NVRM_UNK0005						0x00000005
#define NVRM_FIFO_PIO_NV4					0x0000006d
#define NVRM_FIFO_DMA_NV40					0x0000406e
#define NVRM_FIFO_DMA_NV44					0x0000446e
#define NVRM_FIFO_IB_G80					0x0000506f
#define NVRM_FIFO_IB_G82					0x0000826f
#define NVRM_FIFO_IB_MCP89					0x0000866f
#define NVRM_FIFO_IB_GF100					0x0000906f
#define NVRM_FIFO_IB_GK104					0x0000a06f
#define NVRM_FIFO_IB_GK110					0x0000a16f
#define NVRM_FIFO_IB_UNKA2					0x0000a26f
#define NVRM_FIFO_IB_UNKB0					0x0000b06f
#define NVRM_GR_3D_UNKB1					0x0000b197
#define NVRM_COPY_GT212						0x000085b5
#define NVRM_COPY_GF100_0					0x000090b5
#define NVRM_COPY_GF100_1					0x000090b8
#define NVRM_MPEG_NV17						0x00001774
#define NVRM_MPEG_NV34						0x00003174
#define NVRM_MPEG_G84						0x00008274
#define NVRM_ME_NV40						0x00004075
#define NVRM_VP1_NV41						0x00004176
#define NVRM_VP1_G80						0x00005076
#define NVRM_VP2_G74						0x00007476
#define NVRM_PDEC_G98						0x000088b2
#define NVRM_PDEC_GT212						0x000085b2
#define NVRM_PDEC_GF100						0x000090b2
#define NVRM_PDEC_GF119						0x000095b2
#define NVRM_BSP_G74						0x000074b0
#define NVRM_VLD_G98						0x000088b1
#define NVRM_VLD_GT212						0x000085b1
#define NVRM_VLD_MCP89						0x000086b1
#define NVRM_VLD_GF100						0x000090b1
#define NVRM_VLD_GF119						0x000095b1
#define NVRM_VDEC_GM107						0x0000a0b0
#define NVRM_PPP_G98						0x000088b3
#define NVRM_PPP_GT212						0x000085b3
#define NVRM_PPP_GF100						0x000090b3
#define NVRM_VENC_GK104						0x000090b7
#define NVRM_VENC_GM107						0x0000c0b7
#define NVRM_VCOMP_MCP89					0x000086b6
#define NVRM_SEC_G98						0x000088b4
#define NVRM_SEC_GM107						0x000095a1
#define NVRM_SW_UNK0075						0x00000075
#define NVRM_SW_UNK007D						0x0000007d
#define NVRM_SW_UNK208A						0x0000208a
#define NVRM_SW_UNK5080						0x00005080
#define NVRM_SW_UNK50B0						0x000050b0
#define NVRM_SW_UNK9072						0x00009072
#define NVRM_SW_UNK9074						0x00009074
#define NVRM_DISP_ROOT_G80					0x00005070
#define NVRM_DISP_ROOT_G82					0x00008270
#define NVRM_DISP_ROOT_G200					0x00008370
#define NVRM_DISP_ROOT_G94					0x00008870
#define NVRM_DISP_ROOT_GT212					0x00008570
#define NVRM_DISP_ROOT_GF119					0x00009070
#define NVRM_DISP_ROOT_GK104					0x00009170
#define NVRM_DISP_ROOT_GK110					0x00009270
#define NVRM_DISP_ROOT_GM107					0x00009470
#define NVRM_DISP_FIFO						0x00005079
#define NVRM_DISP_CURSOR_G80					0x0000507a
#define NVRM_DISP_CURSOR_G82					0x0000827a
#define NVRM_DISP_CURSOR_GT212					0x0000857a
#define NVRM_DISP_CURSOR_GF119					0x0000907a
#define NVRM_DISP_CURSOR_GK104					0x0000917a
#define NVRM_DISP_OVPOS_G80					0x0000507b
#define NVRM_DISP_OVPOS_G82					0x0000827b
#define NVRM_DISP_OVPOS_GT212					0x0000857b
#define NVRM_DISP_OVPOS_GF119					0x0000907b
#define NVRM_DISP_OVPOS_GK104					0x0000917b
#define NVRM_DISP_FLIP_G80					0x0000507c
#define NVRM_DISP_FLIP_G82					0x0000827c
#define NVRM_DISP_FLIP_G200					0x0000837c
#define NVRM_DISP_FLIP_GT212					0x0000857c
#define NVRM_DISP_FLIP_GF119					0x0000907c
#define NVRM_DISP_FLIP_GK104					0x0000917c
#define NVRM_DISP_FLIP_GK110					0x0000927c
#define NVRM_DISP_MASTER_G80					0x0000507d
#define NVRM_DISP_MASTER_G82					0x0000827d
#define NVRM_DISP_MASTER_G200					0x0000837d
#define NVRM_DISP_MASTER_G94					0x0000887d
#define NVRM_DISP_MASTER_GT212					0x0000857d
#define NVRM_DISP_MASTER_GF119					0x0000907d
#define NVRM_DISP_MASTER_GK104					0x0000917d
#define NVRM_DISP_MASTER_GK110					0x0000927d
#define NVRM_DISP_MASTER_GM107					0x0000947d
#define NVRM_DISP_OVERLAY_G80					0x0000507e
#define NVRM_DISP_OVERLAY_G82					0x0000827e
#define NVRM_DISP_OVERLAY_G200					0x0000837e
#define NVRM_DISP_OVERLAY_GT212					0x0000857e
#define NVRM_DISP_OVERLAY_GF119					0x0000907e
#define NVRM_DISP_OVERLAY_GK104					0x0000917e
#define NVRM_UNK0001						0x00000001
#define NVRM_UNK0074						0x00000074
#define NVRM_UNK007F						0x0000007f
#define NVRM_UNK402C						0x0000402c
#define NVRM_UNK507F						0x0000507f
#define NVRM_UNK907F						0x0000907f
#define NVRM_UNK50A0						0x000050a0
#define NVRM_UNK50E0						0x000050e0
#define NVRM_UNK50E2						0x000050e2
#define NVRM_UNK824D						0x0000824d
#define NVRM_UNK884D						0x0000884d
#define NVRM_UNK83CC						0x000083cc
#define NVRM_UNK844C						0x0000844c
#define NVRM_UNK9067						0x00009067
#define NVRM_UNK90DD						0x000090dd
#define NVRM_UNK90E0						0x000090e0
#define NVRM_UNK90E1						0x000090e1
#define NVRM_UNK90E2						0x000090e2
#define NVRM_UNK90E3						0x000090e3
#define NVRM_UNK90E4						0x000090e4
#define NVRM_UNK90E5						0x000090e5
#define NVRM_UNK90E6						0x000090e6
#define NVRM_UNK90EC						0x000090ec
#define NVRM_UNK9171						0x00009171
#define NVRM_UNK9271						0x00009271
#define NVRM_UNK9471						0x00009471
#define NVRM_UNKA080						0x0000a080
#define NVRM_UNKA0B6						0x0000a0b6
#define NVRM_UNKA0E0						0x0000a0e0
#define NVRM_UNKA0E1						0x0000a0e1
#define NVRM_UNK0078						0x00000078
#define NVRM_UNK007E						0x0000007e
#define NVRM_UNK00DB						0x000000db
#define NVRM_UNK00F1						0x000000f1
#define NVRM_UNK00FF						0x000000ff
#define NVRM_UNK25A0						0x000025a0
#define NVRM_UNK30F1						0x000030f1
#define NVRM_UNK30F2						0x000030f2
#define NVRM_UNK83F3						0x000083f3
#define NVRM_UNK40CA						0x000040ca
#define NVRM_UNK503B						0x0000503b
#define NVRM_UNK503C						0x0000503c
#define NVRM_UNK5072						0x00005072
#define NVRM_UNK8D75						0x00008d75
#define NVRM_UNK906D						0x0000906d
#define NVRM_UNK906E						0x0000906e
#define NVRM_UNK90F1						0x000090f1
#define NVRM_UNKA06C						0x0000a06c
#define NVRM_UNKA0B7						0x0000a0b7

#endif /* NVRM_OBJECT_XML */
