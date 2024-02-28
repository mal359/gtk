/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include "fnmatch.h"

#include "gdk/gdkkeysyms.h"
#include "gtkbutton.h"
#include "gtkentry.h"
#include "gtkfilesel.h"
#include "gtkhbox.h"
#include "gtkhbbox.h"
#include "gtklabel.h"
#include "gtklist.h"
#include "gtklistitem.h"
#include "gtkmain.h"
#include "gtkscrolledwindow.h"
#include "gtksignal.h"
#include "gtkvbox.h"
#include "gtkmenu.h"
#include "gtkmenuitem.h"
#include "gtkoptionmenu.h"
#include "gtkclist.h"
#include "gtkdialog.h"
#include "gtkintl.h"
#include "gtkpixmap.h"

static char * home_xpm[] = {
"48 48 714 2",
"  	c None",
". 	c #634D3A",
"+ 	c #9C8871",
"@ 	c #654E3D",
"# 	c #35271C",
"$ 	c #000000",
"% 	c #050101",
"& 	c #0B0301",
"* 	c #080100",
"= 	c #0F0402",
"- 	c #B0A08D",
"; 	c #7C6859",
"> 	c #9C8B7C",
", 	c #523D31",
"' 	c #271D16",
") 	c #200806",
"! 	c #C6918A",
"~ 	c #8D2C22",
"{ 	c #541812",
"] 	c #6E1C15",
"^ 	c #020000",
"/ 	c #B6A996",
"( 	c #716052",
"_ 	c #453324",
": 	c #3F2E24",
"< 	c #C3B7B0",
"[ 	c #443529",
"} 	c #1B140F",
"| 	c #110403",
"1 	c #581A14",
"2 	c #49120D",
"3 	c #49130E",
"4 	c #42110C",
"5 	c #0A0101",
"6 	c #A79A87",
"7 	c #837566",
"8 	c #423124",
"9 	c #231B15",
"0 	c #4B423A",
"a 	c #6B5D52",
"b 	c #D1C9C3",
"c 	c #3D3127",
"d 	c #16100C",
"e 	c #371915",
"f 	c #B46B60",
"g 	c #60221C",
"h 	c #69201A",
"i 	c #681F19",
"j 	c #96836F",
"k 	c #9F9185",
"l 	c #5D4A39",
"m 	c #0F0C09",
"n 	c #3A3632",
"o 	c #9A9C99",
"p 	c #453B33",
"q 	c #8A7C71",
"r 	c #C4BAB0",
"s 	c #2E241C",
"t 	c #110C09",
"u 	c #1D0806",
"v 	c #62221C",
"w 	c #541611",
"x 	c #46120D",
"y 	c #A2917D",
"z 	c #AEA397",
"A 	c #806B58",
"B 	c #25201B",
"C 	c #525251",
"D 	c #949493",
"E 	c #656665",
"F 	c #6E6259",
"G 	c #72665C",
"H 	c #D4CDC6",
"I 	c #261D18",
"J 	c #2E0E0B",
"K 	c #A64D41",
"L 	c #603429",
"M 	c #968772",
"N 	c #B7AEA3",
"O 	c #7F6C5D",
"P 	c #0E0B09",
"Q 	c #211C18",
"R 	c #BABAB9",
"S 	c #494948",
"T 	c #B4B5B4",
"U 	c #565655",
"V 	c #B7B9B7",
"W 	c #85807A",
"X 	c #66594F",
"Y 	c #BDB2A7",
"Z 	c #3A2D23",
"` 	c #080504",
" .	c #2B231D",
"..	c #190605",
"+.	c #9F5546",
"@.	c #998A77",
"#.	c #B0A599",
"$.	c #6F6254",
"%.	c #0B0907",
"&.	c #1F1B18",
"*.	c #5C5C5A",
"=.	c #B9B9B7",
"-.	c #40403F",
";.	c #CFD1CF",
">.	c #565656",
",.	c #D2D2D2",
"'.	c #939391",
").	c #918D88",
"!.	c #3C2E25",
"~.	c #A39385",
"{.	c #5C4B40",
"].	c #29211B",
"^.	c #423B32",
"/.	c #9A8B77",
"(.	c #A99D91",
"_.	c #564B40",
":.	c #070605",
"<.	c #1A1715",
"[.	c #A6A6A5",
"}.	c #4F514F",
"|.	c #3A3B3A",
"1.	c #E4E4E4",
"2.	c #606060",
"3.	c #A5A6A5",
"4.	c #9D9D9C",
"5.	c #75716E",
"6.	c #281E17",
"7.	c #A29387",
"8.	c #6F5D4F",
"9.	c #090605",
"0.	c #271F19",
"a.	c #B4A58D",
"b.	c #978777",
"c.	c #BEB5AA",
"d.	c #4B4139",
"e.	c #080605",
"f.	c #1E1B19",
"g.	c #666665",
"h.	c #B1B1B0",
"i.	c #5A5A5A",
"j.	c #BEBEBD",
"k.	c #555555",
"l.	c #EAEAEA",
"m.	c #696969",
"n.	c #E7E7E7",
"o.	c #A9A9A9",
"p.	c #C2C3C2",
"q.	c #686968",
"r.	c #9C9996",
"s.	c #2E221B",
"t.	c #837569",
"u.	c #685446",
"v.	c #AA9A85",
"w.	c #B5A997",
"x.	c #D2CCC3",
"y.	c #3D352D",
"z.	c #080705",
"A.	c #23201C",
"B.	c #B2B2B1",
"C.	c #AEAEAE",
"D.	c #7D7D7D",
"E.	c #DADADA",
"F.	c #777777",
"G.	c #999999",
"H.	c #EBEBEB",
"I.	c #A7A7A7",
"J.	c #DDDDDD",
"K.	c #A5A5A3",
"L.	c #6E6B68",
"M.	c #332921",
"N.	c #68564A",
"O.	c #634D3D",
"P.	c #16100B",
"Q.	c #1B130F",
"R.	c #A0907A",
"S.	c #D1C8B9",
"T.	c #A2968A",
"U.	c #28221C",
"V.	c #0B0908",
"W.	c #4A4641",
"X.	c #828280",
"Y.	c #EFEFEF",
"Z.	c #858585",
"`.	c #E9E9E9",
" +	c #979797",
".+	c #EEEEEE",
"++	c #9A9A9A",
"@+	c #6F6F6F",
"#+	c #D4D4D4",
"$+	c #5F5F5D",
"%+	c #9C9C99",
"&+	c #2D251E",
"*+	c #5D4D40",
"=+	c #6B5545",
"-+	c #2B1F16",
";+	c #C0B199",
">+	c #E7E1D6",
",+	c #756B5F",
"'+	c #18130F",
")+	c #100D0A",
"!+	c #595551",
"~+	c #B7B7B6",
"{+	c #585958",
"]+	c #A9A9A7",
"^+	c #6E6F6E",
"/+	c #C6C8C6",
"(+	c #8E8E8E",
"_+	c #F2F2F2",
":+	c #E8E8E8",
"<+	c #A0A0A0",
"[+	c #9C9C9C",
"}+	c #F0F0F0",
"|+	c #8D8D8D",
"1+	c #777775",
"2+	c #AAAAA9",
"3+	c #74726F",
"4+	c #271F1A",
"5+	c #58483B",
"6+	c #695442",
"7+	c #39291D",
"8+	c #231D1A",
"9+	c #B7A994",
"0+	c #F3ECDA",
"a+	c #DAD4C9",
"b+	c #5F524A",
"c+	c #94918D",
"d+	c #717271",
"e+	c #B6B6B5",
"f+	c #686866",
"g+	c #7A7A78",
"h+	c #969696",
"i+	c #F3F3F3",
"j+	c #B4B4B4",
"k+	c #E5E5E5",
"l+	c #A2A2A2",
"m+	c #EDEDED",
"n+	c #AAAAAA",
"o+	c #D9D9D9",
"p+	c #727271",
"q+	c #93918E",
"r+	c #27211B",
"s+	c #493B30",
"t+	c #715C4B",
"u+	c #463324",
"v+	c #0E0906",
"w+	c #251F1B",
"x+	c #D6C9B2",
"y+	c #FFFBEE",
"z+	c #D2CBC0",
"A+	c #4D4137",
"B+	c #110E0B",
"C+	c #1D1711",
"D+	c #837F7A",
"E+	c #696968",
"F+	c #B5B5B5",
"G+	c #6E6E6C",
"H+	c #D9D9D8",
"I+	c #F4F4F4",
"J+	c #A6A6A6",
"K+	c #E2E2E2",
"L+	c #9D9D9D",
"M+	c #A5A5A5",
"N+	c #A3A3A3",
"O+	c #908E8D",
"P+	c #251F19",
"Q+	c #3A2C24",
"R+	c #6F5C4B",
"S+	c #4F3A2A",
"T+	c #150F0B",
"U+	c #171411",
"V+	c #B4A796",
"W+	c #DACBB2",
"X+	c #FEFAEE",
"Y+	c #DBD3C9",
"Z+	c #352C24",
"`+	c #120F0B",
" @	c #2E261F",
".@	c #8E8E8D",
"+@	c #6C6C6B",
"@@	c #BABBBA",
"#@	c #838383",
"$@	c #ABABAB",
"%@	c #BABABA",
"&@	c #E3E3E3",
"*@	c #ECECEC",
"=@	c #B2B2B2",
"-@	c #DCDCDC",
";@	c #ABADAB",
">@	c #656360",
",@	c #251E17",
"'@	c #2E231B",
")@	c #6E5949",
"!@	c #513B2B",
"~@	c #1F1710",
"{@	c #0D0A08",
"]@	c #C5B7A5",
"^@	c #E5DCCA",
"/@	c #A0948B",
"(@	c #1E1711",
"_@	c #1C1511",
":@	c #514940",
"<@	c #454544",
"[@	c #969694",
"}@	c #B0B0AE",
"|@	c #7F7F7D",
"1@	c #B0B0B0",
"2@	c #C2C2C2",
"3@	c #C8C8C8",
"4@	c #BBBBBB",
"5@	c #B9B9B9",
"6@	c #B7B7B7",
"7@	c #DFDFDF",
"8@	c #B6B6B6",
"9@	c #D3D3D3",
"0@	c #4A4948",
"a@	c #261F1B",
"b@	c #32251C",
"c@	c #543F2E",
"d@	c #493526",
"e@	c #120D09",
"f@	c #16120F",
"g@	c #5F5248",
"h@	c #221912",
"i@	c #554437",
"j@	c #ADA9A6",
"k@	c #3A3A3A",
"l@	c #232322",
"m@	c #444544",
"n@	c #9FA09F",
"o@	c #C5C5C5",
"p@	c #BFBFBF",
"q@	c #BEBEBE",
"r@	c #B1B1B1",
"s@	c #9F9F9F",
"t@	c #808080",
"u@	c #909090",
"v@	c #4B4B4B",
"w@	c #4A4A4A",
"x@	c #2A2A29",
"y@	c #443C36",
"z@	c #291E15",
"A@	c #19120D",
"B@	c #9A9088",
"C@	c #0E0E0E",
"D@	c #1B1B1A",
"E@	c #4A4B4A",
"F@	c #DCDDDC",
"G@	c #F1F1F1",
"H@	c #F5F5F5",
"I@	c #373737",
"J@	c #242424",
"K@	c #080808",
"L@	c #292726",
"M@	c #1C1714",
"N@	c #1F1F1F",
"O@	c #E0E0E0",
"P@	c #D1D1D1",
"Q@	c #CFCFCF",
"R@	c #CECECE",
"S@	c #C9C9C9",
"T@	c #757575",
"U@	c #343434",
"V@	c #1B1B1B",
"W@	c #292929",
"X@	c #545454",
"Y@	c #ADADAD",
"Z@	c #3B3B3B",
"`@	c #D5D5D5",
" #	c #DBDBDB",
".#	c #D8D8D8",
"+#	c #464646",
"@#	c #252828",
"##	c #050505",
"$#	c #0F0E0C",
"%#	c #0C0F0F",
"&#	c #2D302E",
"*#	c #C0C0C0",
"=#	c #BDBDBD",
"-#	c #272727",
";#	c #404040",
">#	c #E1E1E1",
",#	c #DEDEDE",
"'#	c #D7D7D7",
")#	c #181B1B",
"!#	c #374845",
"~#	c #242D2C",
"{#	c #303636",
"]#	c #363D3C",
"^#	c #595D5A",
"/#	c #23221F",
"(#	c #949494",
"_#	c #4F4F4F",
":#	c #222222",
"<#	c #444444",
"[#	c #939393",
"}#	c #262626",
"|#	c #161917",
"1#	c #0A0B0A",
"2#	c #080908",
"3#	c #090B0A",
"4#	c #161716",
"5#	c #303030",
"6#	c #0E0E0C",
"7#	c #2B3534",
"8#	c #555856",
"9#	c #515251",
"0#	c #8E8E8B",
"a#	c #A5A3A0",
"b#	c #0C0C0C",
"c#	c #747474",
"d#	c #3D3D3D",
"e#	c #2E2E2E",
"f#	c #2D2D2D",
"g#	c #525252",
"h#	c #7F7F7F",
"i#	c #151515",
"j#	c #1F221E",
"k#	c #362E2A",
"l#	c #050705",
"m#	c #070C08",
"n#	c #232D25",
"o#	c #393939",
"p#	c #151513",
"q#	c #191F1E",
"r#	c #6F6F6E",
"s#	c #8A8B8A",
"t#	c #CDCCCA",
"u#	c #0A0B0B",
"v#	c #727272",
"w#	c #484848",
"x#	c #2B2B2B",
"y#	c #333333",
"z#	c #424242",
"A#	c #6E6E6E",
"B#	c #1E1E1E",
"C#	c #7A5551",
"D#	c #C2827C",
"E#	c #C4A6A3",
"F#	c #271E1C",
"G#	c #0D0C0A",
"H#	c #313432",
"I#	c #363B37",
"J#	c #CBCBCB",
"K#	c #070808",
"L#	c #171F1E",
"M#	c #7C7D7C",
"N#	c #111313",
"O#	c #666666",
"P#	c #3F3F3F",
"Q#	c #494949",
"R#	c #181615",
"S#	c #974B46",
"T#	c #482521",
"U#	c #B25851",
"V#	c #DEC0BD",
"W#	c #3C3535",
"X#	c #010101",
"Y#	c #262827",
"Z#	c #1C2221",
"`#	c #8B8B8B",
" $	c #D1D2D2",
".$	c #D8D9D8",
"+$	c #080909",
"@$	c #4D4D4D",
"#$	c #5D5D5D",
"$$	c #040202",
"%$	c #391D1A",
"&$	c #7D312B",
"*$	c #B04D45",
"=$	c #C47168",
"-$	c #33201F",
";$	c #292B2A",
">$	c #CDCDCD",
",$	c #CCCCCC",
"'$	c #0E0D0C",
")$	c #212926",
"!$	c #999997",
"~$	c #BBBDBD",
"{$	c #DFE0E0",
"]$	c #141414",
"^$	c #282828",
"/$	c #5F5F5F",
"($	c #414141",
"_$	c #080606",
":$	c #833C34",
"<$	c #8D3F37",
"[$	c #9C413B",
"}$	c #9A413A",
"|$	c #261615",
"1$	c #242524",
"2$	c #CACACA",
"3$	c #1C1E1D",
"4$	c #29322E",
"5$	c #D2D3D2",
"6$	c #D7D7D6",
"7$	c #111111",
"8$	c #919191",
"9$	c #2C2C2C",
"0$	c #0D0D0D",
"a$	c #636363",
"b$	c #9D524A",
"c$	c #9F3C34",
"d$	c #994037",
"e$	c #9C4841",
"f$	c #151716",
"g$	c #6C6C6C",
"h$	c #24201D",
"i$	c #323530",
"j$	c #2E2E28",
"k$	c #343532",
"l$	c #353532",
"m$	c #212121",
"n$	c #3C3C3C",
"o$	c #686868",
"p$	c #0F0F0F",
"q$	c #060606",
"r$	c #8D5651",
"s$	c #9D3D36",
"t$	c #9C453D",
"u$	c #A35149",
"v$	c #2C1F1E",
"w$	c #1B1C1B",
"x$	c #C6C6C6",
"y$	c #BDB4A9",
"z$	c #B0AAA5",
"A$	c #979796",
"B$	c #828282",
"C$	c #626262",
"D$	c #090909",
"E$	c #684946",
"F$	c #8A362E",
"G$	c #97453F",
"H$	c #9C5A54",
"I$	c #261716",
"J$	c #191C1A",
"K$	c #C4C4C4",
"L$	c #C3C3C3",
"M$	c #5C5C5C",
"N$	c #171717",
"O$	c #392423",
"P$	c #632E29",
"Q$	c #96453D",
"R$	c #88544F",
"S$	c #291D1C",
"T$	c #010000",
"U$	c #121513",
"V$	c #1C1C1C",
"W$	c #7A7A7A",
"X$	c #1A1B1B",
"Y$	c #885A56",
"Z$	c #753B36",
"`$	c #804F4B",
" %	c #904F49",
".%	c #282020",
"+%	c #151715",
"@%	c #949790",
"#%	c #626362",
"$%	c #282C23",
"%%	c #36412B",
"&%	c #202E15",
"*%	c #8B8E88",
"=%	c #394131",
"-%	c #A0A59A",
";%	c #1E2020",
">%	c #524442",
",%	c #65403C",
"'%	c #69514E",
")%	c #875551",
"!%	c #2A2424",
"~%	c #070505",
"{%	c #202120",
"]%	c #B0B4AB",
"^%	c #9DA694",
"/%	c #778B65",
"(%	c #6B8258",
"_%	c #6E855A",
":%	c #7F8E6F",
"<%	c #778868",
"[%	c #60774B",
"}%	c #607254",
"|%	c #3F5431",
"1%	c #394B30",
"2%	c #35422E",
"3%	c #30402C",
"4%	c #30412C",
"5%	c #2B3A2B",
"6%	c #2E3F2B",
"7%	c #3C562A",
"8%	c #425F2C",
"9%	c #A2AB99",
"0%	c #3D4A35",
"a%	c #638052",
"b%	c #3B4A30",
"c%	c #21291A",
"d%	c #241918",
"e%	c #4B3533",
"f%	c #6B4F4D",
"g%	c #392D2A",
"h%	c #180C0C",
"i%	c #152215",
"j%	c #161E15",
"k%	c #1F291F",
"l%	c #222C23",
"m%	c #29352B",
"n%	c #2B392D",
"o%	c #2E3F2E",
"p%	c #32452E",
"q%	c #32422E",
"r%	c #2D3B2E",
"s%	c #2A392C",
"t%	c #2D422E",
"u%	c #2A362B",
"v%	c #2C3C2C",
"w%	c #2C3C2D",
"x%	c #2C3B2D",
"y%	c #2B3B2C",
"z%	c #30402A",
"A%	c #394F2B",
"B%	c #45632E",
"C%	c #415F2C",
"D%	c #3F6232",
"E%	c #406634",
"F%	c #416330",
"G%	c #3D592C",
"H%	c #405F30",
"I%	c #3F5F2E",
"J%	c #42482C",
"K%	c #191210",
"L%	c #0F0908",
"M%	c #080505",
"N%	c #070B07",
"O%	c #080D09",
"P%	c #0B0F0B",
"Q%	c #121A0F",
"R%	c #1B2917",
"S%	c #293B1F",
"T%	c #394E2D",
"U%	c #3B5930",
"V%	c #406532",
"W%	c #44652E",
"X%	c #446630",
"Y%	c #354F30",
"Z%	c #29362B",
"`%	c #2A362C",
" &	c #2B3A2C",
".&	c #2C3B2A",
"+&	c #304129",
"@&	c #303F2D",
"#&	c #446C37",
"$&	c #446934",
"%&	c #446332",
"&&	c #405F2D",
"*&	c #416531",
"=&	c #446531",
"-&	c #3D6232",
";&	c #3B562B",
">&	c #374827",
",&	c #3F2B25",
"'&	c #31341F",
")&	c #1D1C11",
"!&	c #1E2312",
"~&	c #253719",
"{&	c #243518",
"]&	c #233317",
"^&	c #2C3D1C",
"/&	c #375125",
"(&	c #42602D",
"_&	c #466630",
":&	c #44632E",
"<&	c #3C552D",
"[&	c #293A28",
"}&	c #2A3B27",
"|&	c #314527",
"1&	c #2C3D22",
"2&	c #1E2A19",
"3&	c #212E1B",
"4&	c #253321",
"5&	c #2A3A26",
"6&	c #446933",
"7&	c #426531",
"8&	c #3C6032",
"9&	c #34512B",
"0&	c #354B23",
"a&	c #27371A",
"b&	c #2B3D1C",
"c&	c #28391B",
"d&	c #334921",
"e&	c #395125",
"f&	c #3D5829",
"g&	c #3A5226",
"h&	c #395129",
"i&	c #3C592D",
"j&	c #3F6332",
"k&	c #42652E",
"l&	c #42632E",
"m&	c #3C592A",
"n&	c #233217",
"o&	c #0B0F09",
"p&	c #26341E",
"q&	c #24321B",
"r&	c #2E4122",
"s&	c #202C1B",
"t&	c #27361E",
"u&	c #233219",
"v&	c #3B5527",
"w&	c #3C5628",
"x&	c #334922",
"y&	c #36542B",
"z&	c #334A22",
"A&	c #30441F",
"B&	c #374F25",
"C&	c #324821",
"D&	c #344A22",
"E&	c #314520",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                .                                               ",
"                                              + @ #                                             ",
"                            $ % & * = $     - ; > , '                                           ",
"                            ) ! ~ { ] ^   / ( _ : < [ }                                         ",
"                            | 1 2 3 4 5 6 7 8 9 0 a b c d                                       ",
"                            e f g h i j k l m n o p q r s t                                     ",
"                            u v w x y z A m B C D E F G H I t                                   ",
"                            J K L M N O P Q R S T U V W X Y Z `  .                              ",
"                            ..+.@.#.$.%.&.*.=.-.;.>.,.'.).!.~.{.` ].                            ",
"                            ^./.(._.:.<.[.}.R |.1.2.1.3.4.5.6.7.8.9.0.                          ",
"                          a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.9.}                         ",
"                        v.w.x.y.z.A.B.U C.D.E.F.l.G.H.I.J.m.K.L.M.N.O.P.Q.                      ",
"                      R.S.T.U.V.W.g.C.}.h.X.Y.Z.`. +.+++H.@+#+$+%+&+*+=+-+`                     ",
"                    ;+>+,+'+)+!+~+{+]+^+/+(+_+I.:+<+.+[+}+|+l.1+2+3+4+5+6+7+` 8+                ",
"                9+0+a+b+)+'+c+d+e+f+e+g+Y.h+i+j+k+l+m+++.+n+`.|+o+p+q+r+s+t+u+v+w+              ",
"              x+y+z+A+B+C+D+h.E+F+G+H+ +I+J+}+n+K+L+l.M+.+N+n.I.1.[+C.O+P+Q+R+S+T+U+            ",
"          V+W+X+Y+Z+`+ @L..@C.+@@@#@i+$@i+j+Y.%@&@I.n.I.*@I.k+C.1.=@-@;@>@,@'@)@!@~@{@          ",
"          ]@^@/@(@_@:@<@C [@}@|@n.1@.+2@}+3@H.4@K+%@K+5@n.6@7@n+-@8@J.9@2.0@a@b@c@d@e@f@        ",
"            g@h@i@j@k@l@m@n@B.j+o@6@p@1@q@r@6@J+=@++L+s@ +t@L+u@I.G.$@1@v@w@x@y@z@A@9           ",
"              B@4@C@$ D@E@F@.+}+G@I+H@i+i+I+H@H@I+_+i+I+I+_+}+H..+:+J.6@I@J@$ K@L@M@$           ",
"                $ $ $ N@k.:+n.`.l.k+K+1.l.`.`.k+1.O@J.o+P@,.,.P@Q@R@S@T@U@V@$ $ $ $ $           ",
"                    $ W@X@o@S@3@4@%@p@5@8@M+J+$@Y@C.n+r@C.o.N+o.L+J+r@i.W@J@$ $ $               ",
"                      Z@X@3@1.1.K+J.-@`@9@#+ #E.o+.#+#@###$#%#&#+#*#=#v@J@-#$                   ",
"                      ;#i.o.&@K+>#7@,#J.-@R@C..#'#Q@)#!#~#{#]#^#/#(#F+_#:#-#$                   ",
"                      <#2.[#>#}#|#1#2#3#4#5#X@r@C.o.6#7#8#9#0#a#b#c#J+d#e#f#$                   ",
"                      g#v@h#o+i#j#k#l#$ m#n#o#`@#+,.p#q#r#h#s#t#u#v#q@w#x#y#$                   ",
"                      2.z#A#,#B#C#D#E#F#G#H#I#J#q@4@K#L#M#h#q@'#N#O#F+P#V@<#$                   ",
"                      A#Q#T@-@R#S#T#U#V#W#X#Y#++h+h+K@Z#`#l+ $.$+$z#F.B#i#@$$                   ",
"                      #$Z@@+o+$$%$&$*$=$-$$ ;$Q@>$,$'$)$!$~${$1.]$>.<+^$b#f#$                   ",
"                      /$($A#o+_$:$<$[$}$|$$ 1$>$,$2$3$4$5$'#7@6$7$k.8$9$0$5#$                   ",
"                      m.e#a$'#K@b$c$d$e$|$$ f$M+++g$h$i$j$3$k$l$m$n$o$-#p$5#                    ",
"                      @+d#P#,.q$r$s$t$u$v$$ w$S@3@x$y$z$A$B$C$a$>.<#O#n$p$5#                    ",
"                      A#>.d#q@D$E$F$G$H$I$$ J$3@o@K$L$2@*#q@=#J+1@C$M$o#N$f#                    ",
"                      c#A#<#Y@D$O$P$Q$R$S$T$U$6@F+n+<+<+[+h+(+t@a$/$w@V$V@9$                    ",
"                        W$n$=@X$Y$Z$`$ %.%X#+%6@8@8@j+r@1@N+ +@%h+Z.#%$%%%&%                    ",
"                        *%=%-%;%>%,%'%)%!%~%{%]%^%/%(%_%:%<%[%}%|%1%2%3%4%5%6%7%8%              ",
"                        9%0%a%b%c%d%e%f%g%h%i%j%k%l%m%n%o%p%q%r%s%t%u%v%w%x%y%z%A%              ",
"                  B%C%D%E%F%G%H%I%J%I$K%L%M%N%O%P%Q%R%S%T%U%V%W%X%Y%Z%`% & &.&+&6%@&            ",
"                #&$&%&&&*&=&*&-&;&>&,&'&)&!&~&{&]&^&/&(&_&_&:&<&[&}&|&1&2&3&4&5&                ",
"                  6&7&8&9&0&a&b&c&d&e&f&g&h&i&j&k&k&l&m&n&  o&p&q&r&s&t&u&                      ",
"                            v&w&x&        y&z&      A&x&B&C&D&E&                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                "};
static char * desktop_xpm[] = {
"48 48 554 2",
"  	c None",
". 	c #D6CCB9",
"+ 	c #DED6C7",
"@ 	c #BFB090",
"# 	c #BFB095",
"$ 	c #9A949A",
"% 	c #BBB5BE",
"& 	c #B5AFB9",
"* 	c #AEA7B1",
"= 	c #AAA3AE",
"- 	c #A69FAA",
"; 	c #A49CA7",
"> 	c #A199A5",
", 	c #A098A4",
"' 	c #A097A4",
") 	c #9F97A3",
"! 	c #A098A5",
"~ 	c #9C94A1",
"{ 	c #908894",
"] 	c #9A8F83",
"^ 	c #776D5A",
"/ 	c #E5E0D4",
"( 	c #F0EDE7",
"_ 	c #E3DDD1",
": 	c #D9D0BD",
"< 	c #D0C6B1",
"[ 	c #746C60",
"} 	c #C0B29D",
"| 	c #F2EFE9",
"1 	c #D0C5B0",
"2 	c #BFAF90",
"3 	c #B8A88C",
"4 	c #38353A",
"5 	c #4C464F",
"6 	c #867B8B",
"7 	c #827787",
"8 	c #817687",
"9 	c #7E7383",
"0 	c #7C7282",
"a 	c #7A6E80",
"b 	c #766A7C",
"c 	c #776B7D",
"d 	c #736879",
"e 	c #746879",
"f 	c #76697C",
"g 	c #74687A",
"h 	c #706376",
"i 	c #6E6174",
"j 	c #6A5D71",
"k 	c #66596D",
"l 	c #5D4F64",
"m 	c #44384B",
"n 	c #3F363B",
"o 	c #B6B3AA",
"p 	c #F8F8F6",
"q 	c #ECE8E0",
"r 	c #CFC3AB",
"s 	c #C4B699",
"t 	c #564F41",
"u 	c #5A4F4B",
"v 	c #BBAF97",
"w 	c #FBFAF8",
"x 	c #C0B192",
"y 	c #BFAF8F",
"z 	c #9F9177",
"A 	c #0F0C10",
"B 	c #281F2B",
"C 	c #514259",
"D 	c #504058",
"E 	c #52425A",
"F 	c #4C3C54",
"G 	c #4B3B53",
"H 	c #483950",
"I 	c #4A3B52",
"J 	c #46374D",
"K 	c #48384F",
"L 	c #4A3A52",
"M 	c #45364C",
"N 	c #403247",
"O 	c #43354B",
"P 	c #221B26",
"Q 	c #433D3A",
"R 	c #F6F4F0",
"S 	c #D1C6B0",
"T 	c #BEAE8F",
"U 	c #4A4338",
"V 	c #443D39",
"W 	c #CEC2AC",
"X 	c #F9F7F5",
"Y 	c #675D52",
"Z 	c #0A070B",
"` 	c #2B212F",
" .	c #493A51",
"..	c #6A6070",
"+.	c #706575",
"@.	c #746979",
"#.	c #9A939D",
"$.	c #B4AFB7",
"%.	c #BBB5BD",
"&.	c #BDB7BF",
"*.	c #E6E4E7",
"=.	c #FDFDFD",
"-.	c #000000",
";.	c #281F2D",
">.	c #221A25",
",.	c #201A21",
"'.	c #E8E3DB",
").	c #C2B396",
"!.	c #3D392E",
"~.	c #363129",
"{.	c #E1D9CB",
"].	c #E9E4DB",
"^.	c #312A2D",
"/.	c #0B080C",
"(.	c #372B3D",
"_.	c #C2BDC5",
":.	c #B8B8B8",
"<.	c #B1B1B1",
"[.	c #C8C8C8",
"}.	c #DBDBDB",
"|.	c #E6E6E6",
"1.	c #EDEDED",
"2.	c #F4F4F4",
"3.	c #F7F7F7",
"4.	c #FBFBFB",
"5.	c #FCFCFC",
"6.	c #FEFEFE",
"7.	c #040305",
"8.	c #2A212F",
"9.	c #252023",
"0.	c #BBB098",
"a.	c #4A4438",
"b.	c #27241D",
"c.	c #D7D2C8",
"d.	c #363237",
"e.	c #181219",
"f.	c #5C4F64",
"g.	c #EAE8EB",
"h.	c #FFFFFF",
"i.	c #A7A7A7",
"j.	c #989898",
"k.	c #BEBEBE",
"l.	c #DCDCDC",
"m.	c #E5E5E5",
"n.	c #EAEAEA",
"o.	c #F5F5F5",
"p.	c #F8F8F8",
"q.	c #555555",
"r.	c #020202",
"s.	c #322835",
"t.	c #5C524B",
"u.	c #36312A",
"v.	c #12100E",
"w.	c #656263",
"x.	c #413945",
"y.	c #423449",
"z.	c #6D6D6D",
"A.	c #707070",
"B.	c #BCBCBC",
"C.	c #D3D3D3",
"D.	c #9B9B9B",
"E.	c #6F6F6F",
"F.	c #797979",
"G.	c #929292",
"H.	c #D1D1D1",
"I.	c #3D333F",
"J.	c #120F15",
"K.	c #C1BDC4",
"L.	c #56475E",
"M.	c #44364B",
"N.	c #3F3246",
"O.	c #D4D4D4",
"P.	c #8F8F8F",
"Q.	c #5D5D5D",
"R.	c #343434",
"S.	c #484848",
"T.	c #5F5F5F",
"U.	c #4E4E4E",
"V.	c #373737",
"W.	c #454545",
"X.	c #A4A4A4",
"Y.	c #DFDFDF",
"Z.	c #B6B6B6",
"`.	c #493951",
" +	c #1C1620",
".+	c #201923",
"++	c #AEA7B0",
"@+	c #B6B1B9",
"#+	c #43354A",
"$+	c #858585",
"%+	c #737373",
"&+	c #787878",
"*+	c #8D8D8D",
"=+	c #959595",
"-+	c #ADADAD",
";+	c #BFBFBF",
">+	c #E8E8E8",
",+	c #F0F0F0",
"'+	c #F9F9F9",
")+	c #717171",
"!+	c #3A2E40",
"~+	c #28212D",
"{+	c #BCB6BE",
"]+	c #A79FAA",
"^+	c #C3C3C3",
"/+	c #ACACAC",
"(+	c #8B8B8B",
"_+	c #5E5E5E",
":+	c #808080",
"<+	c #AAAAAA",
"[+	c #332839",
"}+	c #3D3044",
"|+	c #362B3C",
"1+	c #342939",
"2+	c #C0BAC3",
"3+	c #8F8594",
"4+	c #CECECE",
"5+	c #BBBBBB",
"6+	c #949494",
"7+	c #595959",
"8+	c #282828",
"9+	c #575757",
"0+	c #6A6A6A",
"a+	c #EFEFEF",
"b+	c #E0E0E0",
"c+	c #CFCFCF",
"d+	c #D8D8D8",
"e+	c #281F2C",
"f+	c #35293B",
"g+	c #322737",
"h+	c #C5BFC8",
"i+	c #7C7081",
"j+	c #E1E1E1",
"k+	c #4A4A4A",
"l+	c #AEAEAE",
"m+	c #E9E9E9",
"n+	c #FAFAFA",
"o+	c #C6C6C6",
"p+	c #171219",
"q+	c #C1995E",
"r+	c #D5A960",
"s+	c #735B57",
"t+	c #42344A",
"u+	c #312636",
"v+	c #C5C0C7",
"w+	c #685B6E",
"x+	c #46374E",
"y+	c #EBEBEB",
"z+	c #EEEEEE",
"A+	c #6C6C6C",
"B+	c #C4C4C4",
"C+	c #AD8947",
"D+	c #FECA65",
"E+	c #BE8134",
"F+	c #5A424A",
"G+	c #312736",
"H+	c #BEB9C2",
"I+	c #47384E",
"J+	c #C9C9C9",
"K+	c #7E7E7E",
"L+	c #5C5C5C",
"M+	c #3B3B3B",
"N+	c #4C4C4C",
"O+	c #A9A9A9",
"P+	c #9F9F9F",
"Q+	c #9D9D9D",
"R+	c #FEF9EE",
"S+	c #C59C4E",
"T+	c #EABA62",
"U+	c #CC7300",
"V+	c #84532E",
"W+	c #44354B",
"X+	c #413347",
"Y+	c #47384F",
"Z+	c #332939",
"`+	c #B4AEB7",
" @	c #AFA8B2",
".@	c #F3F3F3",
"+@	c #B2B2B2",
"@@	c #CDCDCD",
"#@	c #ECECEC",
"$@	c #FED076",
"%@	c #E3B45A",
"&@	c #C37413",
"*@	c #AD6510",
"=@	c #392C3E",
"-@	c #392D40",
";@	c #3E3044",
">@	c #47374F",
",@	c #B4ADB7",
"'@	c #A39BA7",
")@	c #493950",
"!@	c #FEF3DD",
"~@	c #D2A456",
"{@	c #652A0C",
"]@	c #2C2231",
"^@	c #2F2535",
"/@	c #2F2534",
"(@	c #322838",
"_@	c #332A37",
":@	c #352938",
"<@	c #B1ABB4",
"[@	c #756C64",
"}@	c #3A2F3D",
"|@	c #3F3145",
"1@	c #444444",
"2@	c #898989",
"3@	c #505050",
"4@	c #3E3E3E",
"5@	c #C3B290",
"6@	c #F2C174",
"7@	c #B06006",
"8@	c #602105",
"9@	c #662100",
"0@	c #2C2230",
"a@	c #241C28",
"b@	c #231B26",
"c@	c #1D1720",
"d@	c #362B3D",
"e@	c #2D2332",
"f@	c #322B30",
"g@	c #6D605B",
"h@	c #2E2432",
"i@	c #B6B0B6",
"j@	c #C2B7A7",
"k@	c #63574D",
"l@	c #3E3245",
"m@	c #D5D5D5",
"n@	c #C7B694",
"o@	c #E6AC5A",
"p@	c #7E3E02",
"q@	c #3A1E1B",
"r@	c #211A25",
"s@	c #18131B",
"t@	c #231B27",
"u@	c #171118",
"v@	c #3B3333",
"w@	c #988A77",
"x@	c #8E8172",
"y@	c #2B2230",
"z@	c #C0B6AF",
"A@	c #F3F0EA",
"B@	c #B4A489",
"C@	c #4A413F",
"D@	c #19141B",
"E@	c #413348",
"F@	c #FEDB98",
"G@	c #FACB71",
"H@	c #DE9E42",
"I@	c #612700",
"J@	c #5E2004",
"K@	c #2A1B21",
"L@	c #1B151E",
"M@	c #1C151E",
"N@	c #0E0A0E",
"O@	c #6E6258",
"P@	c #CDC3B2",
"Q@	c #D5CCC1",
"R@	c #9E907B",
"S@	c #28202D",
"T@	c #E2DDD5",
"U@	c #F3F1EE",
"V@	c #D3C8B3",
"W@	c #C9BCA4",
"X@	c #2A2327",
"Y@	c #120D15",
"Z@	c #33283A",
"`@	c #FDCE73",
" #	c #D58824",
".#	c #BB6C10",
"+#	c #622304",
"@#	c #441600",
"##	c #1C161F",
"$#	c #18121A",
"%#	c #120E14",
"&#	c #141016",
"*#	c #100C10",
"=#	c #998D78",
"-#	c #E9E5E0",
";#	c #EAE6E0",
">#	c #BDAE90",
",#	c #A4957F",
"'#	c #29202D",
")#	c #ABA4AD",
"!#	c #F1EEE8",
"~#	c #EFEBE5",
"{#	c #DDD7C9",
"]#	c #DBD3C3",
"^#	c #BEB197",
"/#	c #302A2B",
"(#	c #09060A",
"_#	c #2A212E",
":#	c #837988",
"<#	c #807C81",
"[#	c #B3ADA2",
"}#	c #EBC274",
"|#	c #7C3D00",
"1#	c #220B00",
"2#	c #100D12",
"3#	c #130F15",
"4#	c #130F16",
"5#	c #100C12",
"6#	c #0A080A",
"7#	c #9D9180",
"8#	c #F3F2F0",
"9#	c #F2EFEA",
"0#	c #CBBFA7",
"a#	c #BAAA8C",
"b#	c #A3947F",
"c#	c #ACA4A6",
"d#	c #F9F8F6",
"e#	c #E3DDCF",
"f#	c #CEC3B0",
"g#	c #BEB099",
"h#	c #C4B7A0",
"i#	c #B2A388",
"j#	c #39342F",
"k#	c #0A090C",
"l#	c #251D29",
"m#	c #17121A",
"n#	c #1B160B",
"o#	c #E1B359",
"p#	c #AF7C2D",
"q#	c #692900",
"r#	c #5A1D00",
"s#	c #110806",
"t#	c #0C090D",
"u#	c #0F0C12",
"v#	c #181414",
"w#	c #A59D91",
"x#	c #F2F0EF",
"y#	c #EDE8E1",
"z#	c #E3DDD0",
"A#	c #C0B19A",
"B#	c #BCAC8D",
"C#	c #988878",
"D#	c #2C2331",
"E#	c #B6ABA2",
"F#	c #F8F7F4",
"G#	c #CFC4AD",
"H#	c #D2C8B3",
"I#	c #C7BAA2",
"J#	c #C1B396",
"K#	c #AB9C80",
"L#	c #AC9D82",
"M#	c #352E2D",
"N#	c #1A151E",
"O#	c #322738",
"P#	c #413349",
"Q#	c #0A080B",
"R#	c #09070A",
"S#	c #403220",
"T#	c #E3B45C",
"U#	c #B47929",
"V#	c #AD5D02",
"W#	c #642102",
"X#	c #4C1C09",
"Y#	c #100C11",
"Z#	c #080609",
"`#	c #0B090D",
" $	c #151017",
".$	c #262120",
"+$	c #A49987",
"@$	c #D4CDC1",
"#$	c #DDD6C9",
"$$	c #E4DED4",
"%$	c #D4CCC0",
"&$	c #CABFAC",
"*$	c #BDAF96",
"=$	c #827469",
"-$	c #584D4B",
";$	c #2D2330",
">$	c #B0A28A",
",$	c #B3B0AC",
"'$	c #565146",
")$	c #464035",
"!$	c #4A4139",
"~$	c #3C352F",
"{$	c #302927",
"]$	c #413936",
"^$	c #161018",
"/$	c #251C28",
"($	c #1E1721",
"_$	c #271F2B",
":$	c #6E583E",
"<$	c #E7B75E",
"[$	c #BF710F",
"}$	c #914C06",
"|$	c #2A110A",
"1$	c #050406",
"2$	c #130E14",
"3$	c #151018",
"4$	c #1C161D",
"5$	c #4E4445",
"6$	c #494044",
"7$	c #8C7E6E",
"8$	c #7C6E66",
"9$	c #766962",
"0$	c #817368",
"a$	c #897B6D",
"b$	c #6B5E5A",
"c$	c #3C3239",
"d$	c #1E1720",
"e$	c #201924",
"f$	c #736B65",
"g$	c #544E4A",
"h$	c #292325",
"i$	c #110E13",
"j$	c #17131A",
"k$	c #1B161F",
"l$	c #211A23",
"m$	c #383037",
"n$	c #241D29",
"o$	c #241E29",
"p$	c #261F2B",
"q$	c #221B25",
"r$	c #928156",
"s$	c #D5A556",
"t$	c #6E2E06",
"u$	c #5C1E01",
"v$	c #0F0705",
"w$	c #030304",
"x$	c #030203",
"y$	c #050405",
"z$	c #1D1721",
"A$	c #211B25",
"B$	c #251E2A",
"C$	c #161118",
"D$	c #D5D68D",
"E$	c #E2BB5D",
"F$	c #C06700",
"G$	c #692100",
"H$	c #4B1800",
"I$	c #BDCC99",
"J$	c #F2ECC5",
"K$	c #B2A96D",
"L$	c #965A05",
"M$	c #A35502",
"N$	c #270C00",
"O$	c #EBE7C0",
"P$	c #CDC398",
"Q$	c #1E1D0F",
"R$	c #0C0900",
"S$	c #512A04",
"T$	c #5E1E00",
"U$	c #0D0400",
"V$	c #9B9D78",
"W$	c #938C6A",
"X$	c #221E17",
"Y$	c #121001",
"Z$	c #391900",
"`$	c #1B1B14",
" %	c #060505",
".%	c #090700",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                . + @ @ @ # $ % & * = - ; > , ' > > ) ' > ! > ! ~ { ] ^ / ( _ : < [             ",
"              } | 1 2 2 3 4 5 6 6 7 8 8 9 0 a b c d e f g h i j k l m n o p q r s t u           ",
"              v w x y z A B C D E F G H I F F F J K G L M N F F F F F O P Q R S T U V           ",
"              W X y Y Z ` F F F F L  .F ..+.@.#.$.%.&.*.=.-.;.F F F F F G >.,.'.).!.~.          ",
"              {.].^./.(.F F F F H _.:.<.[.}.|.1.2.3.4.5.6.-.7.F F F F F F F 8.9.0.a.b.          ",
"              c.d.e.O F F F H f.g.h.i.j.k.l.m.n.o.p.1.=.6.q.r.F F F F F F F L s.t.u.v.          ",
"              w.x.F F L y.M 6 h.h.h.z.A.B.C.D.E.F.G.H.=.h.q.-.F F F F F F L I F I.A J.          ",
"              K.L.M.N.N K F O.P.Q.R.R.S.T.U.V.W.X.Y.Z.h.h.q.-.F F F F F `.F F F F  +.+          ",
"            ++@+#+K G F F F $+%+&+*+=+-+;+>+,+'+5.6.h.h.h.)+-.N F F F I G F F F F !+~+          ",
"            {+]+F F F F F F -+<.Z.^+^+/+(+_+S.A.=+:+,+h.h.<+-.[+F F G L F F F F F }+|+1+        ",
"            2+3+F F F F F F 4+<+5+6+7+8+9+i.0+a+b+c+d+h.h.<+-.e+F I  .F F F F F F y.f+g+        ",
"            h+i+F F F F F G j+k.k+Q.l+m+n+5.Z.p.h.h.h.h.h.o+-.p+q+r+s+F F F F F G t+[+u+        ",
"            v+w+F F F F  .x+y+>+z+2.o.p.5.=.5.-+U.A+B+h.h.h.-.C+D+D+E+F+F G G `.`.x+[+G+        ",
"            H+C F F F H I+F ,+J+E.K+L+M+)+A.N+&+O+P+Q+h.h.R+S+D+T+E+U+V+H W+X+x+Y+x+|+Z+        ",
"          `+ @F F F `.`.F F .@b+n.c++@@@4.h.h.h.h.#@h.h.R+$@D+%@&@U+U+*@=@f+-@;@>@J g+1+        ",
"          ,@'@F F )@G F F F '+n+n+4.=.=.h.X.h.h.h.A.h.!@$@D+~@U+U+U+U+{@]@^@/@(@!+M |+_@:@      ",
"          <@[@}@|@F F F F F n+n+|.1@o+O.2@3@:+A+A+4@5@D+D+6@U+U+U+7@8@9@0@a@b@c@d@e@f@g@h@      ",
"          i@j@k@b@l@F F F F 5.=.4.%+m@5.p.b+h.h.h.n@D+D+o@U+U+U+p@9@9@q@r@c@s@t@u@v@w@x@y@      ",
"          z@A@B@C@D@E@F F F =.6.6.6.h.h.h.h.h.h.F@D+G@H@U+U+U+I@9@J@K@c@L@D@M@N@O@P@Q@R@S@      ",
"          T@U@V@W@X@Y@Z@F F 6.6.6.h.h.h.h.h.h.F@D+`@ #U+U+.#+#9@@###s@$#%#&#*#=#-#;#>#,#'#      ",
"        )#!#~#{#]#^#/#(#_#G :#<#<+<+<+<+<+[#}#D+~@U+U+U+|#9@9@1#2#3#&#4#5#6#7#8#9#0#a#b#y@      ",
"        c#d#e#f#g#h#i#j#k#l#G m#-.-.-.-.n#o#D+p#U+U+U+q#9@r#s#t#t#5#%#u#v#w#x#y#z#A#B#C#D#      ",
"        E#F#G#H#I#J#K#L#M#N#O#P#m#Q#R#S#T#D+U#U+U+V#W#9@X#Y#R#Z#`# $Y#.$+$@$#$$$%$&$*$=$-$;$    ",
"        >$,$'$a.)$!$~${$]$^$/$($t@_$:$D+<$[$U+U+}$9@9@|$Z#1$1$Q#2$3$4$5$6$7$8$9$0$a$b$c$d$e$    ",
"        f$g$h$i$j$N#k$l$m$n$o$p$q$r$D+s$[$U+U+t$9@u$v$w$x$y$R#2#s@z$A$n$B$p$p$p$p$p$p$r@m#C$    ",
"                                  D$E$U+U+F$G$9@H$-.-.-.-.-.                                    ",
"                                I$J$K$L$M$9@9@N$-.-.-.-.-.                                      ",
"                                O$P$Q$R$S$T$U$-.-.-.-.                                          ",
"                              V$W$X$-.Y$Z$-.-.-.-.                                              ",
"                              `$ %.%    -.-.-.                                                  ",
"                                      -.-.                                                      ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                "};

static char * documents_xpm[] = {
"48 48 317 2",
"  	c None",
". 	c #000000",
"+ 	c #978F7C",
"@ 	c #8F8775",
"# 	c #605D52",
"$ 	c #070706",
"% 	c #24231F",
"& 	c #33312C",
"* 	c #3B3932",
"= 	c #3A372F",
"- 	c #676154",
"; 	c #AE8E53",
"> 	c #DDAF5A",
", 	c #F3B94F",
"' 	c #948C79",
") 	c #24221E",
"! 	c #7E7767",
"~ 	c #857E6D",
"{ 	c #524D42",
"] 	c #3E3B34",
"^ 	c #2F2D29",
"/ 	c #201F1D",
"( 	c #191816",
"_ 	c #282622",
": 	c #858175",
"< 	c #A59E8D",
"[ 	c #99927F",
"} 	c #B18F52",
"| 	c #E4B04F",
"1 	c #B48B3A",
"2 	c #6F5224",
"3 	c #DEC277",
"4 	c #CAAF6F",
"5 	c #90825F",
"6 	c #A4A093",
"7 	c #C8C4B9",
"8 	c #D0CDC4",
"9 	c #C5C0B6",
"0 	c #B2AC9E",
"a 	c #A7A090",
"b 	c #7E7867",
"c 	c #6D6758",
"d 	c #22201C",
"e 	c #151411",
"f 	c #181714",
"g 	c #232220",
"h 	c #343432",
"i 	c #43423F",
"j 	c #3D3C38",
"k 	c #524E47",
"l 	c #716B5C",
"m 	c #928A78",
"n 	c #8D8472",
"o 	c #706557",
"p 	c #50473D",
"q 	c #29221E",
"r 	c #201A17",
"s 	c #4D3C1F",
"t 	c #5F4922",
"u 	c #604A22",
"v 	c #E9E089",
"w 	c #FDFAA2",
"x 	c #FAF5A0",
"y 	c #D3C28F",
"z 	c #858481",
"A 	c #C5C2BC",
"B 	c #B6B1A5",
"C 	c #8E8674",
"D 	c #645F54",
"E 	c #57544D",
"F 	c #37352F",
"G 	c #2C2923",
"H 	c #2C2A24",
"I 	c #32302B",
"J 	c #5D5A54",
"K 	c #737069",
"L 	c #A7A59D",
"M 	c #BBB9B1",
"N 	c #A5A297",
"O 	c #A29D93",
"P 	c #857F6E",
"Q 	c #8A8371",
"R 	c #918977",
"S 	c #857B6A",
"T 	c #5B5146",
"U 	c #342D27",
"V 	c #241D17",
"W 	c #4E3D1F",
"X 	c #E5D683",
"Y 	c #E8E392",
"Z 	c #BFBD7C",
"` 	c #312C28",
" .	c #4E4640",
"..	c #605750",
"+.	c #413B34",
"@.	c #413C36",
"#.	c #514B42",
"$.	c #B6B3AD",
"%.	c #B9B5AF",
"&.	c #C0BDB4",
"*.	c #C1BDB2",
"=.	c #BAB5A8",
"-.	c #B5AFA2",
";.	c #B0A99B",
">.	c #A29B8A",
",.	c #958C78",
"'.	c #887E6D",
").	c #675C50",
"!.	c #3C342D",
"~.	c #221B17",
"{.	c #4C3B1F",
"].	c #D0B470",
"^.	c #B8A263",
"/.	c #332E29",
"(.	c #3B3530",
"_.	c #5C524B",
":.	c #585048",
"<.	c #4B433E",
"[.	c #847B75",
"}.	c #695E56",
"|.	c #877F78",
"1.	c #77736D",
"2.	c #EBE6AB",
"3.	c #EEECCF",
"4.	c #DEDDB0",
"5.	c #C1BE70",
"6.	c #9C8653",
"7.	c #9E8B5B",
"8.	c #1F1916",
"9.	c #1E1815",
"0.	c #1E1816",
"a.	c #948259",
"b.	c #3E3731",
"c.	c #3E3732",
"d.	c #534A44",
"e.	c #625850",
"f.	c #574E47",
"g.	c #413A34",
"h.	c #99918B",
"i.	c #463E39",
"j.	c #867E56",
"k.	c #E3E2AB",
"l.	c #C8C7AB",
"m.	c #938E5B",
"n.	c #615233",
"o.	c #3E3522",
"p.	c #181311",
"q.	c #1C1714",
"r.	c #504842",
"s.	c #423B35",
"t.	c #938B85",
"u.	c #332D28",
"v.	c #B6A267",
"w.	c #857F4E",
"x.	c #5F5134",
"y.	c #352D1C",
"z.	c #191412",
"A.	c #5B514A",
"B.	c #504741",
"C.	c #645951",
"D.	c #564D46",
"E.	c #2F2926",
"F.	c #3D352B",
"G.	c #8D7E49",
"H.	c #493D27",
"I.	c #161210",
"J.	c #5D534C",
"K.	c #5E544D",
"L.	c #635851",
"M.	c #594F49",
"N.	c #37302D",
"O.	c #99928C",
"P.	c #3A3430",
"Q.	c #433924",
"R.	c #251F17",
"S.	c #171311",
"T.	c #1A1512",
"U.	c #61564F",
"V.	c #5A514A",
"W.	c #332D29",
"X.	c #9E9792",
"Y.	c #1A1513",
"Z.	c #514943",
"`.	c #5A504A",
" +	c #2C2724",
".+	c #9D9691",
"++	c #332D2A",
"@+	c #3B3531",
"#+	c #645A52",
"$+	c #5F554E",
"%+	c #352F2C",
"&+	c #A29C97",
"*+	c #5F554D",
"=+	c #655A52",
"-+	c #34302B",
";+	c #ABA5A1",
">+	c #2E2926",
",+	c #665B53",
"'+	c #342E2A",
")+	c #A59E9A",
"!+	c #0C0908",
"~+	c #595049",
"{+	c #595048",
"]+	c #282421",
"^+	c #9F9893",
"/+	c #624C27",
"(+	c #231A0C",
"_+	c #47403A",
":+	c #514842",
"<+	c #27221F",
"[+	c #AAA39F",
"}+	c #382C1D",
"|+	c #E3AF4D",
"1+	c #372A12",
"2+	c #48403B",
"3+	c #4D443F",
"4+	c #554C46",
"5+	c #231F1C",
"6+	c #AFA9A5",
"7+	c #403833",
"8+	c #B3893E",
"9+	c #CE9C43",
"0+	c #2B200E",
"a+	c #3A342F",
"b+	c #48413B",
"c+	c #5A5049",
"d+	c #23201D",
"e+	c #AAA4A0",
"f+	c #61574F",
"g+	c #755928",
"h+	c #423214",
"i+	c #140F06",
"j+	c #18130C",
"k+	c #39332F",
"l+	c #36312C",
"m+	c #4A423C",
"n+	c #37312D",
"o+	c #9E9995",
"p+	c #5D544C",
"q+	c #4F4741",
"r+	c #665C54",
"s+	c #040303",
"t+	c #0B0904",
"u+	c #71613A",
"v+	c #242018",
"w+	c #413A35",
"x+	c #524A43",
"y+	c #2F2A27",
"z+	c #999490",
"A+	c #120F0D",
"B+	c #030302",
"C+	c #D1CD7A",
"D+	c #454025",
"E+	c #1C1916",
"F+	c #4A433D",
"G+	c #86807E",
"H+	c #60564E",
"I+	c #3F3732",
"J+	c #1D1714",
"K+	c #D6CD7A",
"L+	c #A69557",
"M+	c #2A251D",
"N+	c #352F2B",
"O+	c #403934",
"P+	c #84807D",
"Q+	c #625750",
"R+	c #3D3531",
"S+	c #1B1613",
"T+	c #38311C",
"U+	c #9B8652",
"V+	c #84724C",
"W+	c #292421",
"X+	c #3C3631",
"Y+	c #7F7A76",
"Z+	c #322C28",
"`+	c #181411",
" @	c #1C1614",
".@	c #0F0C0B",
"+@	c #070604",
"@@	c #2D281D",
"#@	c #1D1A17",
"$@	c #302B28",
"%@	c #696561",
"&@	c #544B45",
"*@	c #433C37",
"=@	c #5E5240",
"-@	c #938151",
";@	c #2F271D",
">@	c #1D1815",
",@	c #1C1917",
"'@	c #36302C",
")@	c #726D6B",
"!@	c #554C45",
"~@	c #6F6349",
"{@	c #E4D985",
"]@	c #B9AF67",
"^@	c #6A5D39",
"/@	c #31291C",
"(@	c #221C16",
"_@	c #1D1B1A",
":@	c #49423B",
"<@	c #C2B570",
"[@	c #F7F49D",
"}@	c #C4BE71",
"|@	c #635333",
"1@	c #493E24",
"2@	c #1C1710",
"3@	c #49413B",
"4@	c #4A4131",
"5@	c #B6A864",
"6@	c #C3AC63",
"7@	c #78673D",
"8@	c #252012",
"9@	c #030202",
"0@	c #2A2419",
"a@	c #342D1B",
"b@	c #17130C",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                    .                                           ",
"                                          . . . . . . . .                                       ",
"                                        . . . . . . . . .                                       ",
"                                        . .             .                                       ",
"                                      . .         + @ # $ % & * = - +   ; > ,                   ",
"                                  + ' ) ) ! ~ { ] ^ / ( _ : < [ + + + + } | 1 2                 ",
"          3 4 5 6 7 8 9 0 a + + + b c d e f g h i j k l m + + n o p q r r s t u                 ",
"      v w x y z A B C D E F G H I J K L M N O P Q R n S T U r r r r r r r V W t                 ",
"      X Y Z `  ...+.@.#.$.%.&.*.=.-.;.>.+ ,.'.).!.r r r r r r r r r r r r r ~.{.                ",
"      ].^./.(._.:.<.[.}.|.1.2.3.4.5.6.7.8.9.0.r r r r r r r r r r r r r r r r r                 ",
"      a.b.c.d.e.f.g.h.}.}.i.j.k.l.m.n.o.p.q.r r r r r r r r r r r r r r r r r                   ",
"        r.<._.}.f.s.t.}.}.}.u.v.w.x.y.z.p.8.r r r r r r r r r r r r r r r r r                   ",
"        A.B.C.}.D.E.t.}.}.}.d.F.G.H.z.I.q.r r r r r r r r r r r r r r r r r r                   ",
"        J.K.L.}.M.N.O.}.}.}.}.P.Q.R.S.T.r r r r r r r r r r r r r r r r r r r                   ",
"        A.U.}.}.V.W.X.}.}.}.}.A.Y.T.I.r r r r r r r r r r r r r r r r r r r r                   ",
"        Z._.e.}.`. +.+}.}.}.}._.r 9.q.r r r r r r r r r r r r r r r r r r r r                   ",
"        i.U.U.}._.++O.}.}.}.}._.r r r r r r r r r r r r r r r r r r r r r r r                   ",
"        @+#+$+}._.%+&+}.}.}.}._.r r r r r r r r r r r r r r r r r r r r r r r                   ",
"         +*+$+=+D.-+;+}.}.}.}._.r r r r r r r r r r r r r r r r r r r r r r 9.                  ",
"        >+K._.,+V.'+)+}.}.}.}._.r r r r r r r r r r r r r r r r r r r r r 0.!+                  ",
"          ~+~+C.{+]+^+}.}.}.}._.r r r r r r r r r r r r r r r r r r r r r /+(+                  ",
"          _+:+A.`.<+[+}.}.}.}._.r r r r r r r r r r r r r r r r r r r r }+|+1+                  ",
"        . 2+3+4+_.5+6+}.}.}.}.7+r r r r r r r r r r r r r r r r r r r r 8+9+0+                  ",
"          a+:+b+c+d+e+}.}.f+~+7+r r r r r r r r r r r r r r r r r r r V g+h+i+. . .             ",
"        . j+k+l+m+n+o+}.p+q+r+7+r r r r r r r r r r r r r r r r r r p.s+. t+. . . . . . .       ",
"          u+v+w+x+y+z+K.d._.d.7+r r r r r r r r r r r r r r r r A+B+. . . . . . . . . . . .     ",
"          C+D+E+F+W.G+,+H+f.f+I+J+8.r r r r r r r r r r r r A+B+. . . . . . . . . . . . .       ",
"        . K+L+M+N+O+P+L._.A.Q+R+q.S+9.r r r r r r r r r A+B+. . . . . . . . . . . . .           ",
"        . T+U+V+W+X+Y+#+f.A.q+Z+z.`+Y. @r r r r r q..@B+. . . . . . . . . . . . .               ",
"          . +@@@#@$@%@H+&@*@=@-@;@I.z.Y.>@r r q..@. . . . . . . . . . . . . .                   ",
"              . ,@'@)@!@*@~@{@]@^@/@(@q.r q..@. . . . . . . . . . . . . .                       ",
"                  . _@a+:@<@[@}@|@1@2@I..@. . . . . . . . . . . . .                             ",
"                        3@4@5@6@7@8@9@. . . . . . . . . . . . .                                 ",
"                            0@a@b@. . . . . . . . . .                                           ",
"                                . . . .                                                         ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                ",
"                                                                                                "};

char *text;

#define DIR_LIST_WIDTH   180
#define DIR_LIST_HEIGHT  180
#define FILE_LIST_WIDTH  180
#define FILE_LIST_HEIGHT 180

/* The Hurd doesn't define either PATH_MAX or MAXPATHLEN, so we put this
 * in here, since the rest of the code in the file does require some
 * fixed maximum.
 */
#ifndef MAXPATHLEN
#  ifdef PATH_MAX
#    define MAXPATHLEN PATH_MAX
#  else
#    define MAXPATHLEN 2048
#  endif
#endif

/* I've put this here so it doesn't get confused with the 
 * file completion interface */
typedef struct _HistoryCallbackArg HistoryCallbackArg;

struct _HistoryCallbackArg
{
  gchar *directory;
  GtkWidget *menu_item;
};


typedef struct _CompletionState    CompletionState;
typedef struct _CompletionDir      CompletionDir;
typedef struct _CompletionDirSent  CompletionDirSent;
typedef struct _CompletionDirEntry CompletionDirEntry;
typedef struct _CompletionUserDir  CompletionUserDir;
typedef struct _PossibleCompletion PossibleCompletion;

/* Non-external file completion decls and structures */

/* A contant telling PRCS how many directories to cache.  Its actually
 * kept in a list, so the geometry isn't important. */
#define CMPL_DIRECTORY_CACHE_SIZE 10

/* A constant used to determine whether a substring was an exact
 * match by first_diff_index()
 */
#define PATTERN_MATCH -1
/* The arguments used by all fnmatch() calls below
 */
#define FNMATCH_FLAGS (FNM_PATHNAME | FNM_PERIOD)

#define CMPL_ERRNO_TOO_LONG ((1<<16)-1)

/* This structure contains all the useful information about a directory
 * for the purposes of filename completion.  These structures are cached
 * in the CompletionState struct.  CompletionDir's are reference counted.
 */
/* Saves errno when something cmpl does fails. */
static gint cmpl_errno;

struct _CompletionDirSent
{
  ino_t inode;
  time_t mtime;
  dev_t device;

  gint entry_count;
  gchar *name_buffer; /* memory segment containing names of all entries */

  struct _CompletionDirEntry *entries;
};

struct _CompletionDir
{
  CompletionDirSent *sent;

  gchar *fullname;
  gint fullname_len;

  struct _CompletionDir *cmpl_parent;
  gint cmpl_index;
  gchar *cmpl_text;
};

/* This structure contains pairs of directory entry names with a flag saying
 * whether or not they are a valid directory.  NOTE: This information is used
 * to provide the caller with information about whether to update its completions
 * or try to open a file.  Since directories are cached by the directory mtime,
 * a symlink which points to an invalid file (which will not be a directory),
 * will not be reevaluated if that file is created, unless the containing
 * directory is touched.  I consider this case to be worth ignoring (josh).
 */
struct _CompletionDirEntry
{
  gint is_dir;
  gchar *entry_name;
};

struct _CompletionUserDir
{
  gchar *login;
  gchar *homedir;
};

struct _PossibleCompletion
{
  /* accessible fields, all are accessed externally by functions
   * declared above
   */
  gchar *text;
  gint is_a_completion;
  gint is_directory;

  /* Private fields
   */
  gint text_alloc;
};

struct _CompletionState
{
  gint last_valid_char;
  gchar *updated_text;
  gint updated_text_len;
  gint updated_text_alloc;
  gint re_complete;

  gchar *user_dir_name_buffer;
  gint user_directories_len;

  gchar *last_completion_text;

  gint user_completion_index; /* if >= 0, currently completing ~user */

  struct _CompletionDir *completion_dir; /* directory completing from */
  struct _CompletionDir *active_completion_dir;

  struct _PossibleCompletion the_completion;

  struct _CompletionDir *reference_dir; /* initial directory */

  GList* directory_storage;
  GList* directory_sent_storage;

  struct _CompletionUserDir *user_directories;
};

static CompletionDir*
attach_dir(CompletionDirSent* sent, gchar* dir_name, 
	  CompletionState *cmpl_state)
{
  CompletionDir* new_dir;

  new_dir = g_new(CompletionDir, 1);

  cmpl_state->directory_storage =
    g_list_prepend(cmpl_state->directory_storage, new_dir);

  new_dir->sent = sent;
  new_dir->fullname = g_strdup(dir_name);
  new_dir->fullname_len = strlen(dir_name);

  return new_dir;
}

static gint first_diff_index(gchar* pat, gchar* text);
static gint compare_user_dir(const void* a, const void* b);
static gint compare_cmpl_dir(const void* a, const void* b)
{
  return strcmp((((CompletionDirEntry*)a))->entry_name,
		(((CompletionDirEntry*)b))->entry_name);
}
static void update_cmpl(PossibleCompletion* poss,
			CompletionState* cmpl_state);

static CompletionDirSent* open_new_dir
(gchar* dir_name, struct stat* sbuf, gboolean stat_subdirs)
{
  CompletionDirSent* sent;
  DIR* directory;
  gchar *buffer_ptr;
  struct dirent *dirent_ptr;
  gint buffer_size = 0;
  gint entry_count = 0;
  gint i;
  struct stat ent_sbuf;
  char path_buf[MAXPATHLEN*2];
  gint path_buf_len;

  sent = g_new(CompletionDirSent, 1);
  sent->mtime = sbuf->st_mtime;
  sent->inode = sbuf->st_ino;
  sent->device = sbuf->st_dev;

  path_buf_len = strlen(dir_name);

  if (path_buf_len > MAXPATHLEN)
    {
      cmpl_errno = CMPL_ERRNO_TOO_LONG;
      return NULL;
    }

  strcpy(path_buf, dir_name);

  directory = opendir(dir_name);

  if(!directory)
    {
      cmpl_errno = errno;
      return NULL;
    }

  while((dirent_ptr = readdir(directory)) != NULL)
    {
      int entry_len = strlen(dirent_ptr->d_name);
      buffer_size += entry_len + 1;
      entry_count += 1;

      if(path_buf_len + entry_len + 2 >= MAXPATHLEN)
	{
	  cmpl_errno = CMPL_ERRNO_TOO_LONG;
 	  closedir(directory);
	  return NULL;
	}
    }

  sent->name_buffer = g_new(gchar, buffer_size);
  sent->entries = g_new(CompletionDirEntry, entry_count);
  sent->entry_count = entry_count;

  buffer_ptr = sent->name_buffer;

  rewinddir(directory);

  for(i = 0; i < entry_count; i += 1)
    {
      dirent_ptr = readdir(directory);

      if(!dirent_ptr)
	{
	  cmpl_errno = errno;
	  closedir(directory);
	  return NULL;
	}

      strcpy(buffer_ptr, dirent_ptr->d_name);
      sent->entries[i].entry_name = buffer_ptr;
      buffer_ptr += strlen(dirent_ptr->d_name);
      *buffer_ptr = 0;
      buffer_ptr += 1;

      path_buf[path_buf_len] = '/';
      strcpy(path_buf + path_buf_len + 1, dirent_ptr->d_name);

      if (stat_subdirs)
	{
	  if(stat(path_buf, &ent_sbuf) >= 0 && S_ISDIR(ent_sbuf.st_mode))
	    sent->entries[i].is_dir = 1;
	  else
	    /* stat may fail, and we don't mind, since it could be a
	     * dangling symlink. */
	    sent->entries[i].is_dir = 0;
	}
      else
	sent->entries[i].is_dir = 1;
    }

  qsort(sent->entries, sent->entry_count, sizeof(CompletionDirEntry), compare_cmpl_dir);

  closedir(directory);

  return sent;
}


/* Directory operations. */
/* after the cache lookup fails, really open a new directory */

static gboolean check_dir(gchar *dir_name, struct stat *result, gboolean *stat_subdirs)
{
  /* A list of directories that we know only contain other directories.
   * Trying to stat every file in these directories would be very
   * expensive.
   */

  static struct {
    gchar *name;
    gboolean present;
    struct stat statbuf;
  } no_stat_dirs[] = {
    { "/afs", FALSE, { 0 } },
    { "/net", FALSE, { 0 } }
  };

  static const gint n_no_stat_dirs = sizeof(no_stat_dirs) / sizeof(no_stat_dirs[0]);
  static gboolean initialized = FALSE;

  gint i;

  if (!initialized)
    {
      initialized = TRUE;
      for (i = 0; i < n_no_stat_dirs; i++)
	{
	  if (stat (no_stat_dirs[i].name, &no_stat_dirs[i].statbuf) == 0)
	    no_stat_dirs[i].present = TRUE;
	}
    }

  if(stat(dir_name, result) < 0)
    {
      cmpl_errno = errno;
      return FALSE;
    }

  *stat_subdirs = TRUE;
  for (i=0; i<n_no_stat_dirs; i++)
    {
      if (no_stat_dirs[i].present &&
	  (no_stat_dirs[i].statbuf.st_dev == result->st_dev) &&
	  (no_stat_dirs[i].statbuf.st_ino == result->st_ino))
	{
	  *stat_subdirs = FALSE;
	  break;
	}
    }

  return TRUE;
}
/* open a directory by absolute pathname */
static CompletionDir* open_dir(gchar* dir_name, CompletionState* cmpl_state)
{
  struct stat sbuf;
  gboolean stat_subdirs;
  CompletionDirSent *sent;
  GList* cdsl;

  if (!check_dir (dir_name, &sbuf, &stat_subdirs))
    return NULL;

  cdsl = cmpl_state->directory_sent_storage;

  while (cdsl)
    {
      sent = cdsl->data;

      if(sent->inode == sbuf.st_ino &&
	 sent->mtime == sbuf.st_mtime &&
	 sent->device == sbuf.st_dev)
	return attach_dir(sent, dir_name, cmpl_state);

      cdsl = cdsl->next;
    }

  sent = open_new_dir(dir_name, &sbuf, stat_subdirs);

  if (sent) {
    cmpl_state->directory_sent_storage =
      g_list_prepend(cmpl_state->directory_sent_storage, sent);

    return attach_dir(sent, dir_name, cmpl_state);
  }

  return NULL;
}

static CompletionDir* open_user_dir        (gchar* text_to_complete,
					    CompletionState *cmpl_state)
{
  gchar *first_slash;
  gint cmp_len;

  g_assert(text_to_complete && text_to_complete[0] == '~');

  first_slash = strchr(text_to_complete, '/');

  if (first_slash)
    cmp_len = first_slash - text_to_complete - 1;
  else
    cmp_len = strlen(text_to_complete + 1);

  if(!cmp_len)
    {
      /* ~/ */
      gchar *homedir = g_get_home_dir ();

      if (homedir)
	return open_dir(homedir, cmpl_state);
      else
	return NULL;
    }
  else
    {
      /* ~user/ */
      char* copy = g_new(char, cmp_len + 1);
      struct passwd *pwd;
      strncpy(copy, text_to_complete + 1, cmp_len);
      copy[cmp_len] = 0;
      pwd = getpwnam(copy);
      g_free(copy);
      if (!pwd)
	{
	  cmpl_errno = errno;
	  return NULL;
	}

      return open_dir(pwd->pw_dir, cmpl_state);
    }
}
static CompletionDir* open_ref_dir(gchar* text_to_complete,
	     gchar** remaining_text,
	     CompletionState* cmpl_state)
{
  gchar* first_slash;
  CompletionDir *new_dir;

  first_slash = strchr(text_to_complete, '/');

  if (text_to_complete[0] == '~')
    {
      new_dir = open_user_dir(text_to_complete, cmpl_state);

      if(new_dir)
	{
	  if(first_slash)
	    *remaining_text = first_slash + 1;
	  else
	    *remaining_text = text_to_complete + strlen(text_to_complete);
	}
      else
	{
	  return NULL;
	}
    }
  else if (text_to_complete[0] == '/' || !cmpl_state->reference_dir)
    {
      gchar *tmp = g_strdup(text_to_complete);
      gchar *p;

      p = tmp;
      while (*p && *p != '*' && *p != '?')
	p++;

      *p = '\0';
      p = strrchr(tmp, '/');
      if (p)
	{
	  if (p == tmp)
	    p++;
      
	  *p = '\0';

	  new_dir = open_dir(tmp, cmpl_state);

	  if(new_dir)
	    *remaining_text = text_to_complete + 
	      ((p == tmp + 1) ? (p - tmp) : (p + 1 - tmp));
	}
      else
	{
	  /* If no possible candidates, use the cwd */
	  gchar *curdir = g_get_current_dir ();
	  
	  new_dir = open_dir(curdir, cmpl_state);

	  if (new_dir)
	    *remaining_text = text_to_complete;

	  g_free (curdir);
	}

      g_free (tmp);
    }
  else
    {
      *remaining_text = text_to_complete;

      new_dir = open_dir(cmpl_state->reference_dir->fullname, cmpl_state);
    }

  if(new_dir)
    {
      new_dir->cmpl_index = -1;
      new_dir->cmpl_parent = NULL;
    }

  return new_dir;
}
static CompletionDir* open_relative_dir    (gchar* dir_name, CompletionDir* dir,
					    CompletionState *cmpl_state)
{
  gchar path_buf[2*MAXPATHLEN];

  if(dir->fullname_len + strlen(dir_name) + 2 >= MAXPATHLEN)
    {
      cmpl_errno = CMPL_ERRNO_TOO_LONG;
      return NULL;
    }

  strcpy(path_buf, dir->fullname);

  if(dir->fullname_len > 1)
    {
      path_buf[dir->fullname_len] = '/';
      strcpy(path_buf + dir->fullname_len + 1, dir_name);
    }
  else
    {
      strcpy(path_buf + dir->fullname_len, dir_name);
    }

  return open_dir(path_buf, cmpl_state);
}


/* File completion functions which would be external, were they used
 * outside of this file.
 */

static CompletionState*    cmpl_init_state        (void)
{
  gchar getcwd_buf[2*MAXPATHLEN];
  CompletionState *new_state;
  gint fallback = 0;

  new_state = g_new (CompletionState, 1);

  /* We don't use getcwd() on SUNOS, because, it does a popen("pwd")
   * and, if that wasn't bad enough, hangs in doing so.
   */
#if defined(sun) && !defined(__SVR4)
  if (!getwd (getcwd_buf))
#else    
  if (!getcwd (getcwd_buf, MAXPATHLEN))
#endif    
    {
      /* Oh joy, we can't get the current directory. Um..., we should have
       * a root directory, right? Right? (Probably not portable to non-Unix)
       */
      strcpy (getcwd_buf, "/");
      fallback = 1;
    }

tryagain:

  new_state->reference_dir = NULL;
  new_state->completion_dir = NULL;
  new_state->active_completion_dir = NULL;
  new_state->directory_storage = NULL;
  new_state->directory_sent_storage = NULL;
  new_state->last_valid_char = 0;
  new_state->updated_text = g_new (gchar, MAXPATHLEN);
  new_state->updated_text_alloc = MAXPATHLEN;
  new_state->the_completion.text = g_new (gchar, MAXPATHLEN);
  new_state->the_completion.text_alloc = MAXPATHLEN;
  new_state->user_dir_name_buffer = NULL;
  new_state->user_directories = NULL;

  new_state->reference_dir =  open_dir (getcwd_buf, new_state);

  if (!new_state->reference_dir)
    {
      /* Directories changing from underneath us, grumble */
      if (fallback == 0) {
	strcpy (getcwd_buf, "/");
	fallback = 1;
      }
      else {
	if (fallback == 1) { /* last try with user home dir */
	  if (g_get_home_dir ()) {
	    strcpy (getcwd_buf, g_get_home_dir ());
	  }
	  fallback = 2;
	}
	else {
	  g_error ("no readable fallback directory found for GtkFileSelector, exiting");
	}
      }
      /* don't leak updated_text and the_completion.text */
      if (new_state->the_completion.text)
	g_free (new_state->the_completion.text);
      if (new_state->updated_text)
	g_free (new_state->updated_text);
      goto tryagain;
    }

  return new_state;
}

static void                cmpl_free_state        (CompletionState *cmpl_state);

static void           free_dir_sent (CompletionDirSent* sent)
{
  g_free(sent->name_buffer);
  g_free(sent->entries);
  g_free(sent);
}
static void           free_dir      (CompletionDir  *dir)
{
  g_free(dir->fullname);
  g_free(dir);
}

/**********************************************************************/
/*	                 Construction, deletion                       */
/**********************************************************************/

void cmpl_free_dir_list(GList* dp0)
{
  GList *dp = dp0;

  while (dp) {
    free_dir (dp->data);
    dp = dp->next;
  }

  g_list_free(dp0);
}

void cmpl_free_dir_sent_list(GList* dp0)
{
  GList *dp = dp0;

  while (dp) {
    free_dir_sent (dp->data);
    dp = dp->next;
  }

  g_list_free(dp0);
}

void cmpl_free_state (CompletionState* cmpl_state)
{
  cmpl_free_dir_list (cmpl_state->directory_storage);
  cmpl_free_dir_sent_list (cmpl_state->directory_sent_storage);

  if (cmpl_state->user_dir_name_buffer)
    g_free (cmpl_state->user_dir_name_buffer);
  if (cmpl_state->user_directories)
    g_free (cmpl_state->user_directories);
  if (cmpl_state->the_completion.text)
    g_free (cmpl_state->the_completion.text);
  if (cmpl_state->updated_text)
    g_free (cmpl_state->updated_text);

  g_free (cmpl_state);
}

static void           prune_memory_usage(CompletionState *cmpl_state)
{
  GList* cdsl = cmpl_state->directory_sent_storage;
  GList* cdl = cmpl_state->directory_storage;
  GList* cdl0 = cdl;
  gint len = 0;

  for(; cdsl && len < CMPL_DIRECTORY_CACHE_SIZE; len += 1)
    cdsl = cdsl->next;

  if (cdsl) {
    cmpl_free_dir_sent_list(cdsl->next);
    cdsl->next = NULL;
  }

  cmpl_state->directory_storage = NULL;
  while (cdl) {
    if (cdl->data == cmpl_state->reference_dir)
      cmpl_state->directory_storage = g_list_prepend(NULL, cdl->data);
    else
      free_dir (cdl->data);
    cdl = cdl->next;
  }

  g_list_free(cdl0);
}





/* The three completion selectors
 *
 *
 * Returns a name for consideration, possibly a completion, this name
 * will be invalid after the next call to cmpl_next_completion.
 */
static char*               cmpl_this_completion   (PossibleCompletion* pc)
{
  return pc->text;
}

/* True if this completion matches the given text.  Otherwise, this
 * output can be used to have a list of non-completions.
 */
static gint                cmpl_is_a_completion   (PossibleCompletion* pc)
{
  return pc->is_a_completion;
}

/* True if the completion is a directory
 */
static gint                cmpl_is_directory      (PossibleCompletion* pc)
{
  return pc->is_directory;
}


/* The four completion state selectors
 *
 *
 * Updating completions: the return value of cmpl_updated_text() will
 * be text_to_complete completed as much as possible after the most
 * recent call to cmpl_completion_matches.  For the present
 * application, this is the suggested replacement for the user's input
 * string.  You must CALL THIS AFTER ALL cmpl_text_completions have
 * been received.
 */
static gchar*              cmpl_updated_text       (CompletionState* cmpl_state)
{
  return cmpl_state->updated_text;
}

/* After updating, to see if the completion was a directory, call
 * this.  If it was, you should consider re-calling completion_matches.
 */
static gint                cmpl_updated_dir        (CompletionState* cmpl_state)
{
  return cmpl_state->re_complete;
}

/* Current location: if using file completion, return the current
 * directory, from which file completion begins.  More specifically,
 * the cwd concatenated with all exact completions up to the last
 * directory delimiter('/').
 */
static gchar*              cmpl_reference_position (CompletionState* cmpl_state)
{
  return cmpl_state->reference_dir->fullname;
}

/* backing up: if cmpl_completion_matches returns NULL, you may query
 * the index of the last completable character into cmpl_updated_text.
 */
static gint                cmpl_last_valid_char    (CompletionState* cmpl_state)
{
  return cmpl_state->last_valid_char;
}

static gint compare_user_dir(const void* a, const void* b)
{
  return strcmp((((CompletionUserDir*)a))->login,
		(((CompletionUserDir*)b))->login);
}

static gint cmpl_state_okay(CompletionState* cmpl_state)
{
  return  cmpl_state && cmpl_state->reference_dir;
}

static gchar* cmpl_strerror(gint err)
{
  if(err == CMPL_ERRNO_TOO_LONG)
    return "Name too long";
  else
    return g_strerror (err);
}

				    
/* When the user selects a non-directory, call cmpl_completion_fullname
 * to get the full name of the selected file.
 */
static gchar*              cmpl_completion_fullname (gchar*, CompletionState* cmpl_state)
{
  static char nothing[2] = "";

  if (!cmpl_state_okay (cmpl_state))
    {
      return nothing;
    }
  else if (text[0] == '/')
    {
      strcpy (cmpl_state->updated_text, text);
    }
  else if (text[0] == '~')
    {
      CompletionDir* dir;
      char* slash;

      dir = open_user_dir (text, cmpl_state);

      if (!dir)
	{
	  /* spencer says just return ~something, so
	   * for now just do it. */
	  strcpy (cmpl_state->updated_text, text);
	}
      else
	{

	  strcpy (cmpl_state->updated_text, dir->fullname);

	  slash = strchr (text, '/');

	  if (slash)
	    strcat (cmpl_state->updated_text, slash);
	}
    }
  else
    {
      strcpy (cmpl_state->updated_text, cmpl_state->reference_dir->fullname);
      if (strcmp (cmpl_state->reference_dir->fullname, "/") != 0)
	strcat (cmpl_state->updated_text, "/");
      strcat (cmpl_state->updated_text, text);
    }

  return cmpl_state->updated_text;
}

static gchar* find_parent_dir_fullname(gchar* dirname)
{
  gchar buffer[MAXPATHLEN];
  gchar buffer2[MAXPATHLEN];

#if defined(sun) && !defined(__SVR4)
  if(!getwd(buffer))
#else
  if(!getcwd(buffer, MAXPATHLEN))
#endif    
    {
      cmpl_errno = errno;
      return NULL;
    }

  if(chdir(dirname) != 0 || chdir("..") != 0)
    {
      cmpl_errno = errno;
      return NULL;
    }

#if defined(sun) && !defined(__SVR4)
  if(!getwd(buffer2))
#else
  if(!getcwd(buffer2, MAXPATHLEN))
#endif
    {
      chdir(buffer);
      cmpl_errno = errno;

      return NULL;
    }

  if(chdir(buffer) != 0)
    {
      cmpl_errno = errno;
      return NULL;
    }

  return g_strdup(buffer2);
}

static gint correct_parent(CompletionDir* cmpl_dir, struct stat *sbuf)
{
  struct stat parbuf;
  gchar *last_slash;
  gchar *new_name;
  gchar c = 0;

  last_slash = strrchr(cmpl_dir->fullname, '/');

  g_assert(last_slash);

  if(last_slash != cmpl_dir->fullname)
    { /* last_slash[0] = 0; */ }
  else
    {
      c = last_slash[1];
      last_slash[1] = 0;
    }

  if (stat(cmpl_dir->fullname, &parbuf) < 0)
    {
      cmpl_errno = errno;
      return FALSE;
    }

  if (parbuf.st_ino == sbuf->st_ino && parbuf.st_dev == sbuf->st_dev)
    /* it wasn't a link */
    return TRUE;

  if(c)
    last_slash[1] = c;
  /* else
    last_slash[0] = '/'; */

  /* it was a link, have to figure it out the hard way */

  new_name = find_parent_dir_fullname(cmpl_dir->fullname);

  if (!new_name)
    return FALSE;

  g_free(cmpl_dir->fullname);

  cmpl_dir->fullname = new_name;

  return TRUE;
}
static gint correct_dir_fullname(CompletionDir* cmpl_dir)
{
  gint length = strlen(cmpl_dir->fullname);
  struct stat sbuf;

  if (strcmp(cmpl_dir->fullname + length - 2, "/.") == 0)
    {
      if (length == 2) 
	{
	  strcpy(cmpl_dir->fullname, "/");
	  cmpl_dir->fullname_len = 1;
	  return TRUE;
	} else {
	  cmpl_dir->fullname[length - 2] = 0;
	}
    }
  else if (strcmp(cmpl_dir->fullname + length - 3, "/./") == 0)
    cmpl_dir->fullname[length - 2] = 0;
  else if (strcmp(cmpl_dir->fullname + length - 3, "/..") == 0)
    {
      if(length == 3)
	{
	  strcpy(cmpl_dir->fullname, "/");
	  cmpl_dir->fullname_len = 1;
	  return TRUE;
	}

      if(stat(cmpl_dir->fullname, &sbuf) < 0)
	{
	  cmpl_errno = errno;
	  return FALSE;
	}

      cmpl_dir->fullname[length - 2] = 0;

      if(!correct_parent(cmpl_dir, &sbuf))
	return FALSE;
    }
  else if (strcmp(cmpl_dir->fullname + length - 4, "/../") == 0)
    {
      if(length == 4)
	{
	  strcpy(cmpl_dir->fullname, "/");
	  cmpl_dir->fullname_len = 1;
	  return TRUE;
	}

      if(stat(cmpl_dir->fullname, &sbuf) < 0)
	{
	  cmpl_errno = errno;
	  return FALSE;
	}

      cmpl_dir->fullname[length - 3] = 0;

      if(!correct_parent(cmpl_dir, &sbuf))
	return FALSE;
    }

  cmpl_dir->fullname_len = strlen(cmpl_dir->fullname);

  return TRUE;
}
static CompletionDir* attach_dir           (CompletionDirSent* sent,
					    gchar* dir_name,
					    CompletionState *cmpl_state);

static gint get_pwdb(CompletionState* cmpl_state)
{
  struct passwd *pwd_ptr;
  gchar* buf_ptr;
  gint len = 0, i, count = 0;

  if(cmpl_state->user_dir_name_buffer)
    return TRUE;
  setpwent ();

  while ((pwd_ptr = getpwent()) != NULL)
    {
      len += strlen(pwd_ptr->pw_name);
      len += strlen(pwd_ptr->pw_dir);
      len += 2;
      count += 1;
    }

  setpwent ();

  cmpl_state->user_dir_name_buffer = g_new(gchar, len);
  cmpl_state->user_directories = g_new(CompletionUserDir, count);
  cmpl_state->user_directories_len = count;

  buf_ptr = cmpl_state->user_dir_name_buffer;

  for(i = 0; i < count; i += 1)
    {
      pwd_ptr = getpwent();
      if(!pwd_ptr)
	{
	  cmpl_errno = errno;
	  goto error;
	}

      strcpy(buf_ptr, pwd_ptr->pw_name);
      cmpl_state->user_directories[i].login = buf_ptr;
      buf_ptr += strlen(buf_ptr);
      buf_ptr += 1;
      strcpy(buf_ptr, pwd_ptr->pw_dir);
      cmpl_state->user_directories[i].homedir = buf_ptr;
      buf_ptr += strlen(buf_ptr);
      buf_ptr += 1;
    }

  qsort(cmpl_state->user_directories,
	cmpl_state->user_directories_len,
	sizeof(CompletionUserDir),
	compare_user_dir);

  endpwent();

  return TRUE;

error:

  if(cmpl_state->user_dir_name_buffer)
    g_free(cmpl_state->user_dir_name_buffer);
  if(cmpl_state->user_directories)
    g_free(cmpl_state->user_directories);

  cmpl_state->user_dir_name_buffer = NULL;
  cmpl_state->user_directories = NULL;

  return FALSE;
}
/**********************************************************************/
/*                        Completion Operations                       */
/**********************************************************************/

/* returns the index (>= 0) of the first differing character,
 * PATTERN_MATCH if the completion matches */
static gint first_diff_index(gchar* pat, gchar* text)
{
  gint diff = 0;

  while(*pat && *text && *text == *pat)
    {
      pat += 1;
      text += 1;
      diff += 1;
    }

  if(*pat)
    return diff;

  return PATTERN_MATCH;
}

static PossibleCompletion* append_completion_text(gchar* text, CompletionState* cmpl_state)
{
  gint len, i = 1;

  if(!cmpl_state->the_completion.text)
    return NULL;

  len = strlen(text) + strlen(cmpl_state->the_completion.text) + 1;

  if(cmpl_state->the_completion.text_alloc > len)
    {
      strcat(cmpl_state->the_completion.text, text);
      return &cmpl_state->the_completion;
    }

  while(i < len) { i <<= 1; }

  cmpl_state->the_completion.text_alloc = i;

  cmpl_state->the_completion.text = (gchar*)g_realloc(cmpl_state->the_completion.text, i);

  if(!cmpl_state->the_completion.text)
    return NULL;
  else
    {
      strcat(cmpl_state->the_completion.text, text);
      return &cmpl_state->the_completion;
    }
}

static CompletionDir* find_completion_dir(gchar* text_to_complete,
		    gchar** remaining_text,
		    CompletionState* cmpl_state)
{
  gchar* first_slash = strchr(text_to_complete, '/');
  CompletionDir* dir = cmpl_state->reference_dir;
  CompletionDir* next;
  *remaining_text = text_to_complete;

  while(first_slash)
    {
      gint len = first_slash - *remaining_text;
      gint found = 0;
      gchar *found_name = NULL;         /* Quiet gcc */
      gint i;
      gchar* pat_buf = g_new (gchar, len + 1);

      strncpy(pat_buf, *remaining_text, len);
      pat_buf[len] = 0;

      for(i = 0; i < dir->sent->entry_count; i += 1)
	{
	  if(dir->sent->entries[i].is_dir &&
	     fnmatch(pat_buf, dir->sent->entries[i].entry_name,
		     FNMATCH_FLAGS)!= FNM_NOMATCH)
	    {
	      if(found)
		{
		  g_free (pat_buf);
		  return dir;
		}
	      else
		{
		  found = 1;
		  found_name = dir->sent->entries[i].entry_name;
		}
	    }
	}

      if (!found)
	{
	  /* Perhaps we are trying to open an automount directory */
	  found_name = pat_buf;
	}

      next = open_relative_dir(found_name, dir, cmpl_state);
      
      if(!next)
	{
	  g_free (pat_buf);
	  return NULL;
	}
      
      next->cmpl_parent = dir;
      
      dir = next;
      
      if(!correct_dir_fullname(dir))
	{
	  g_free(pat_buf);
	  return NULL;
	}
      
      *remaining_text = first_slash + 1;
      first_slash = strchr(*remaining_text, '/');

      g_free (pat_buf);
    }

  return dir;
}

static void update_cmpl(PossibleCompletion* poss, CompletionState* cmpl_state)
{
  gint cmpl_len;

  if(!poss || !cmpl_is_a_completion(poss))
    return;

  cmpl_len = strlen(cmpl_this_completion(poss));

  if(cmpl_state->updated_text_alloc < cmpl_len + 1)
    {
      cmpl_state->updated_text =
	(gchar*)g_realloc(cmpl_state->updated_text,
			  cmpl_state->updated_text_alloc);
      cmpl_state->updated_text_alloc = 2*cmpl_len;
    }

  if(cmpl_state->updated_text_len < 0)
    {
      strcpy(cmpl_state->updated_text, cmpl_this_completion(poss));
      cmpl_state->updated_text_len = cmpl_len;
      cmpl_state->re_complete = cmpl_is_directory(poss);
    }
  else if(cmpl_state->updated_text_len == 0)
    {
      cmpl_state->re_complete = FALSE;
    }
  else
    {
      gint first_diff =
	first_diff_index(cmpl_state->updated_text,
			 cmpl_this_completion(poss));

      cmpl_state->re_complete = FALSE;

      if(first_diff == PATTERN_MATCH)
	return;

      if(first_diff > cmpl_state->updated_text_len)
	strcpy(cmpl_state->updated_text, cmpl_this_completion(poss));

      cmpl_state->updated_text_len = first_diff;
      cmpl_state->updated_text[first_diff] = 0;
    }
}

static PossibleCompletion* attempt_file_completion(CompletionState *cmpl_state)
{
  gchar *pat_buf, *first_slash;
  CompletionDir *dir = cmpl_state->active_completion_dir;

  dir->cmpl_index += 1;

  if(dir->cmpl_index == dir->sent->entry_count)
    {
      if(dir->cmpl_parent == NULL)
	{
	  cmpl_state->active_completion_dir = NULL;

	  return NULL;
	}
      else
	{
	  cmpl_state->active_completion_dir = dir->cmpl_parent;

	  return attempt_file_completion(cmpl_state);
	}
    }

  g_assert(dir->cmpl_text);

  first_slash = strchr(dir->cmpl_text, '/');

  if(first_slash)
    {
      gint len = first_slash - dir->cmpl_text;

      pat_buf = g_new (gchar, len + 1);
      strncpy(pat_buf, dir->cmpl_text, len);
      pat_buf[len] = 0;
    }
  else
    {
      gint len = strlen(dir->cmpl_text);

      pat_buf = g_new (gchar, len + 2);
      strcpy(pat_buf, dir->cmpl_text);
      strcpy(pat_buf + len, "*");
    }

  if(first_slash)
    {
      if(dir->sent->entries[dir->cmpl_index].is_dir)
	{
	  if(fnmatch(pat_buf, dir->sent->entries[dir->cmpl_index].entry_name,
		     FNMATCH_FLAGS) != FNM_NOMATCH)
	    {
	      CompletionDir* new_dir;

	      new_dir = open_relative_dir(dir->sent->entries[dir->cmpl_index].entry_name,
					  dir, cmpl_state);

	      if(!new_dir)
		{
		  g_free (pat_buf);
		  return NULL;
		}

	      new_dir->cmpl_parent = dir;

	      new_dir->cmpl_index = -1;
	      new_dir->cmpl_text = first_slash + 1;

	      cmpl_state->active_completion_dir = new_dir;

	      g_free (pat_buf);
	      return attempt_file_completion(cmpl_state);
	    }
	  else
	    {
	      g_free (pat_buf);
	      return attempt_file_completion(cmpl_state);
	    }
	}
      else
	{
	  g_free (pat_buf);
	  return attempt_file_completion(cmpl_state);
	}
    }
  else
    {
      if(dir->cmpl_parent != NULL)
	{
	  append_completion_text(dir->fullname +
				 strlen(cmpl_state->completion_dir->fullname) + 1,
				 cmpl_state);
	  append_completion_text("/", cmpl_state);
	}

      append_completion_text(dir->sent->entries[dir->cmpl_index].entry_name, cmpl_state);

      cmpl_state->the_completion.is_a_completion =
	(fnmatch(pat_buf, dir->sent->entries[dir->cmpl_index].entry_name,
		 FNMATCH_FLAGS) != FNM_NOMATCH);

      cmpl_state->the_completion.is_directory = dir->sent->entries[dir->cmpl_index].is_dir;
      if(dir->sent->entries[dir->cmpl_index].is_dir)
	append_completion_text("/", cmpl_state);

      g_free (pat_buf);
      return &cmpl_state->the_completion;
    }
}

/* Completion operations */
static PossibleCompletion* attempt_homedir_completion(gchar* text_to_complete,
						      CompletionState *cmpl_state)
{
  gint index, length;

  if (!cmpl_state->user_dir_name_buffer &&
      !get_pwdb(cmpl_state))
    return NULL;
  length = strlen(text_to_complete) - 1;

  cmpl_state->user_completion_index += 1;

  while(cmpl_state->user_completion_index < cmpl_state->user_directories_len)
    {
      index = first_diff_index(text_to_complete + 1,
			       cmpl_state->user_directories
			       [cmpl_state->user_completion_index].login);

      switch(index)
	{
	case PATTERN_MATCH:
	  break;
	default:
	  if(cmpl_state->last_valid_char < (index + 1))
	    cmpl_state->last_valid_char = index + 1;
	  cmpl_state->user_completion_index += 1;
	  continue;
	}

      cmpl_state->the_completion.is_a_completion = 1;
      cmpl_state->the_completion.is_directory = 1;

      append_completion_text("~", cmpl_state);

      append_completion_text(cmpl_state->
			      user_directories[cmpl_state->user_completion_index].login,
			     cmpl_state);

      return append_completion_text("/", cmpl_state);
    }

  if(text_to_complete[1] ||
     cmpl_state->user_completion_index > cmpl_state->user_directories_len)
    {
      cmpl_state->user_completion_index = -1;
      return NULL;
    }
  else
    {
      cmpl_state->user_completion_index += 1;
      cmpl_state->the_completion.is_a_completion = 1;
      cmpl_state->the_completion.is_directory = 1;

      return append_completion_text("~/", cmpl_state);
    }
}

static PossibleCompletion* cmpl_completion_matches(gchar           *text_to_complete,
						   gchar          **remaining_text,
						   CompletionState *cmpl_state)
{
  gchar* first_slash;
  PossibleCompletion *poss;

  prune_memory_usage(cmpl_state);

  g_assert (text_to_complete != NULL);

  cmpl_state->user_completion_index = -1;
  cmpl_state->last_completion_text = text_to_complete;
  cmpl_state->the_completion.text[0] = 0;
  cmpl_state->last_valid_char = 0;
  cmpl_state->updated_text_len = -1;
  cmpl_state->updated_text[0] = 0;
  cmpl_state->re_complete = FALSE;

  first_slash = strchr (text_to_complete, '/');

  if (text_to_complete[0] == '~' && !first_slash)
    {
      /* Text starts with ~ and there is no slash, show all the
       * home directory completions.
       */
      poss = attempt_homedir_completion (text_to_complete, cmpl_state);

      update_cmpl(poss, cmpl_state);

      return poss;
    }

  cmpl_state->reference_dir =
    open_ref_dir (text_to_complete, remaining_text, cmpl_state);

  if(!cmpl_state->reference_dir)
    return NULL;

  cmpl_state->completion_dir =
    find_completion_dir (*remaining_text, remaining_text, cmpl_state);

  cmpl_state->last_valid_char = *remaining_text - text_to_complete;

  if(!cmpl_state->completion_dir)
    return NULL;

  cmpl_state->completion_dir->cmpl_index = -1;
  cmpl_state->completion_dir->cmpl_parent = NULL;
  cmpl_state->completion_dir->cmpl_text = *remaining_text;

  cmpl_state->active_completion_dir = cmpl_state->completion_dir;

  cmpl_state->reference_dir = cmpl_state->completion_dir;

  poss = attempt_file_completion(cmpl_state);

  update_cmpl(poss, cmpl_state);

  return poss;
}


static PossibleCompletion* attempt_file_completion(CompletionState *cmpl_state);
static CompletionDir* find_completion_dir(gchar* text_to_complete,
					  gchar** remaining_text,
					  CompletionState* cmpl_state);
static PossibleCompletion* append_completion_text(gchar* text,
						  CompletionState* cmpl_state);

static void gtk_file_selection_class_init    (GtkFileSelectionClass *klass);
static void gtk_file_selection_init          (GtkFileSelection      *filesel);
static void gtk_file_selection_destroy       (GtkObject             *object);
static gint gtk_file_selection_key_press     (GtkWidget             *widget,
					      GdkEventKey           *event,
					      gpointer               user_data);

static void gtk_file_selection_file_button (GtkWidget *widget,
					    gint row, 
					    gint column, 
					    GdkEventButton *bevent,
					    gpointer user_data);

static void gtk_file_selection_dir_button (GtkWidget *widget,
					   gint row, 
					   gint column, 
					   GdkEventButton *bevent,
					   gpointer data);

static void gtk_file_selection_populate      (GtkFileSelection      *fs,
					      gchar                 *rel_path,
					      gboolean               try_complete,
					      gboolean               reset_entry);
					      
static void gtk_file_selection_abort         (GtkFileSelection      *fs)
{
  gchar err_buf[256];

  sprintf (err_buf, _("Directory unreadable: %s"), cmpl_strerror (cmpl_errno));

  /*  BEEP gdk_beep();  */

  if (fs->selection_entry)
    gtk_label_set_text (GTK_LABEL (fs->selection_text), err_buf);
}

static void gtk_file_selection_update_history_menu (GtkFileSelection       *fs,
						    gchar                  *current_dir);

static void gtk_file_selection_create_dir (GtkWidget *widget, gpointer data);
static void gtk_file_selection_delete_file (GtkWidget *widget, gpointer data);
static void gtk_file_selection_rename_file (GtkWidget *widget, gpointer data);

static void home_clicked (GtkWidget *widget, gpointer data);
static void desktop_clicked (GtkWidget *widget, gpointer data);
static void documents_clicked (GtkWidget *widget, gpointer data);


static GtkWindowClass *parent_class = NULL;

static char *last_dir = NULL;

GtkType
gtk_file_selection_get_type (void)
{
  static GtkType file_selection_type = 0;

  if (!file_selection_type)
    {
      static const GtkTypeInfo filesel_info =
      {
	"GtkFileSelection",
	sizeof (GtkFileSelection),
	sizeof (GtkFileSelectionClass),
	(GtkClassInitFunc) gtk_file_selection_class_init,
	(GtkObjectInitFunc) gtk_file_selection_init,
	/* reserved_1 */ NULL,
	/* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      file_selection_type = gtk_type_unique (GTK_TYPE_WINDOW, &filesel_info);
    }

  return file_selection_type;
}

static void
gtk_file_selection_class_init (GtkFileSelectionClass *class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass*) class;

  parent_class = gtk_type_class (GTK_TYPE_WINDOW);

  object_class->destroy = gtk_file_selection_destroy;
}

static GtkWidget *
create_pixmap_button (GtkFileSelection *filesel, 
		      GtkWidget *parent_hbox,
		      gchar **xpm_data,
		      gchar *title)
{
  GtkWidget *btn;
  GtkWidget *hbox;
  GtkWidget *wpixmap;
  GtkWidget *label;
  GdkPixmap *pixmap;
  GdkPixmap *mask;
  
  btn = gtk_button_new ();
  gtk_box_pack_start (GTK_BOX (parent_hbox), btn, TRUE, FALSE, 5);
  gtk_widget_realize (btn);

  pixmap = gdk_pixmap_create_from_xpm_d (btn->window, &mask, 
					 &btn->style->bg[GTK_STATE_NORMAL], 
					 (gchar**)xpm_data);
  
  label = gtk_label_new (title);
  
  wpixmap = gtk_pixmap_new (pixmap, mask);
  
  hbox = gtk_vbox_new (FALSE, 5);

  gtk_box_pack_start (GTK_BOX (hbox), wpixmap, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);

  gtk_widget_show_all (hbox);

  gtk_container_add (GTK_CONTAINER (btn), hbox);

  return btn;
}

static void
gtk_file_selection_init (GtkFileSelection *filesel)
{
  GtkWidget *entry_vbox;
  GtkWidget *label;
  GtkWidget *list_hbox;
  GtkWidget *confirm_area;
  GtkWidget *pulldown_hbox;
  GtkWidget *scrolled_win;
  GtkWidget *navigation_area;
  GtkWidget *btn;

  char *dirname;
  char *dir_title [2];
  char *file_title [2];
  
  filesel->cmpl_state = cmpl_init_state ();

  /* The dialog-sized vertical box  */
  filesel->main_vbox = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (filesel), 10);
  gtk_container_add (GTK_CONTAINER (filesel), filesel->main_vbox);
  gtk_widget_show (filesel->main_vbox);

  /* hbox for pulldown menu */
  pulldown_hbox = gtk_hbox_new (TRUE, 5);
  gtk_box_pack_start (GTK_BOX (filesel->main_vbox), pulldown_hbox, FALSE, FALSE, 0);
  gtk_widget_show (pulldown_hbox);
  
  /* Pulldown menu */
  filesel->history_pulldown = gtk_option_menu_new ();
  gtk_widget_show (filesel->history_pulldown);
  gtk_box_pack_start (GTK_BOX (pulldown_hbox), filesel->history_pulldown, 
		      TRUE, FALSE, 0);
    
  /*  The horizontal box containing the directory and file listboxes  */
  list_hbox = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (filesel->main_vbox), list_hbox, TRUE, TRUE, 0);
  gtk_widget_show (list_hbox);

  /* The navigation buttons */
  navigation_area = gtk_vbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (list_hbox), navigation_area, FALSE, FALSE, 5);
  gtk_widget_show (navigation_area);

  if (g_get_home_dir () != NULL) 
    {
      struct stat sb;
      btn = create_pixmap_button (filesel, navigation_area, 
				  (gchar **)home_xpm, 
				  _("Home"));
      gtk_widget_show (btn);
      gtk_signal_connect (GTK_OBJECT (btn), "clicked",
			  home_clicked, filesel);

      dirname = g_strdup_printf ("%s/.gnome-desktop/", g_get_home_dir ());
      if (stat (dirname, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
	  btn = create_pixmap_button (filesel, navigation_area, 
				      (gchar **)desktop_xpm, 
				      _("Desktop"));
	  gtk_widget_show (btn);
	  gtk_signal_connect (GTK_OBJECT (btn), "clicked",
			      desktop_clicked, filesel);

	  
	}
      g_free (dirname);
      
      dirname = g_strdup_printf ("%s/Documents/", g_get_home_dir ());
      if (stat (dirname, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
	  btn = create_pixmap_button (filesel, navigation_area, 
				      (gchar **)documents_xpm, 
				      _("Documents"));
	  gtk_widget_show (btn);
	  gtk_signal_connect (GTK_OBJECT (btn), "clicked",
			      documents_clicked, filesel);
	}
      
      g_free (dirname);
    }

  /* The directories clist */
  dir_title[0] = _("Directories");
  dir_title[1] = NULL;
  filesel->dir_list = gtk_clist_new_with_titles (1, (gchar**) dir_title);
  gtk_widget_set_usize (filesel->dir_list, DIR_LIST_WIDTH, DIR_LIST_HEIGHT);
  gtk_signal_connect (GTK_OBJECT (filesel->dir_list), "select_row",
		      (GtkSignalFunc) gtk_file_selection_dir_button, 
		      (gpointer) filesel);
  gtk_clist_column_titles_passive (GTK_CLIST (filesel->dir_list));

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_add (GTK_CONTAINER (scrolled_win), filesel->dir_list);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_win), 5);
  gtk_box_pack_start (GTK_BOX (list_hbox), scrolled_win, TRUE, TRUE, 0);
  gtk_widget_show (filesel->dir_list);
  gtk_widget_show (scrolled_win);

  /* The files clist */
  file_title[0] = _("Files");
  file_title[1] = NULL;
  filesel->file_list = gtk_clist_new_with_titles (1, (gchar**) file_title);
  gtk_widget_set_usize (filesel->file_list, FILE_LIST_WIDTH, FILE_LIST_HEIGHT);
  gtk_signal_connect (GTK_OBJECT (filesel->file_list), "select_row",
		      (GtkSignalFunc) gtk_file_selection_file_button, 
		      (gpointer) filesel);
  gtk_clist_column_titles_passive (GTK_CLIST (filesel->file_list));

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_add (GTK_CONTAINER (scrolled_win), filesel->file_list);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_win), 5);
  gtk_box_pack_start (GTK_BOX (list_hbox), scrolled_win, TRUE, TRUE, 0);
  gtk_widget_show (filesel->file_list);
  gtk_widget_show (scrolled_win);

  /* The horizontal box containing create, rename etc. buttons */
  filesel->button_area = gtk_hbutton_box_new ();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(filesel->button_area), GTK_BUTTONBOX_END);
  gtk_button_box_set_spacing(GTK_BUTTON_BOX(filesel->button_area), 0);
  gtk_box_pack_start (GTK_BOX (filesel->main_vbox), filesel->button_area, 
		      FALSE, FALSE, 0);
  gtk_widget_show (filesel->button_area);
  
  gtk_file_selection_show_fileop_buttons(filesel);

  /* action area for packing buttons into. */
  filesel->action_area = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (filesel->main_vbox), filesel->action_area, 
		      FALSE, FALSE, 0);
  gtk_widget_show (filesel->action_area);
  
  /*  The OK/Cancel button area */
  confirm_area = gtk_hbutton_box_new ();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(confirm_area), GTK_BUTTONBOX_END);
  gtk_button_box_set_spacing(GTK_BUTTON_BOX(confirm_area), 5);
  gtk_box_pack_end (GTK_BOX (filesel->main_vbox), confirm_area, FALSE, FALSE, 0);
  gtk_widget_show (confirm_area);

  /*  The OK button  */
  filesel->ok_button = gtk_button_new_with_label (_("OK"));
  GTK_WIDGET_SET_FLAGS (filesel->ok_button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (confirm_area), filesel->ok_button, TRUE, TRUE, 0);
  gtk_widget_grab_default (filesel->ok_button);
  gtk_widget_show (filesel->ok_button);

  /*  The Cancel button  */
  filesel->cancel_button = gtk_button_new_with_label (_("Cancel"));
  GTK_WIDGET_SET_FLAGS (filesel->cancel_button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (confirm_area), filesel->cancel_button, TRUE, TRUE, 0);
  gtk_widget_show (filesel->cancel_button);

  /*  The selection entry widget  */
  entry_vbox = gtk_vbox_new (FALSE, 2);
  gtk_box_pack_end (GTK_BOX (filesel->main_vbox), entry_vbox, FALSE, FALSE, 0);
  gtk_widget_show (entry_vbox);

  filesel->selection_text = label = gtk_label_new ("");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_box_pack_start (GTK_BOX (entry_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  filesel->selection_entry = gtk_entry_new ();
  gtk_signal_connect (GTK_OBJECT (filesel->selection_entry), "key_press_event",
		      (GtkSignalFunc) gtk_file_selection_key_press, filesel);
  gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "focus_in_event",
			     (GtkSignalFunc) gtk_widget_grab_default,
			     GTK_OBJECT (filesel->ok_button));
  gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "activate",
                             (GtkSignalFunc) gtk_button_clicked,
                             GTK_OBJECT (filesel->ok_button));
  gtk_box_pack_start (GTK_BOX (entry_vbox), filesel->selection_entry, TRUE, TRUE, 0);
  gtk_object_set_data (GTK_OBJECT (filesel->selection_entry), "last_entry_was_dir_selection", (gpointer) FALSE);
  gtk_widget_show (filesel->selection_entry);

  if (!cmpl_state_okay (filesel->cmpl_state))
    {
      gchar err_buf[256];

      sprintf (err_buf, _("Directory unreadable: %s"), cmpl_strerror (cmpl_errno));

      gtk_label_set_text (GTK_LABEL (filesel->selection_text), err_buf);
    }
  else
    {
      gtk_file_selection_populate (filesel, "", FALSE, TRUE);
    }

  gtk_widget_grab_focus (filesel->selection_entry);
}

GtkWidget*
gtk_file_selection_new (const gchar *title)
{
  GtkFileSelection *filesel;

  filesel = gtk_type_new (GTK_TYPE_FILE_SELECTION);
  gtk_window_set_title (GTK_WINDOW (filesel), title);

  return GTK_WIDGET (filesel);
}

void
gtk_file_selection_show_fileop_buttons (GtkFileSelection *filesel)
{
  g_return_if_fail (filesel != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (filesel));
    
  /* delete, create directory, and rename */
  if (!filesel->fileop_c_dir) 
    {
      filesel->fileop_c_dir = gtk_button_new_with_label (_("Create Dir"));
      gtk_signal_connect (GTK_OBJECT (filesel->fileop_c_dir), "clicked",
			  (GtkSignalFunc) gtk_file_selection_create_dir, 
			  (gpointer) filesel);
      gtk_box_pack_start (GTK_BOX (filesel->button_area), 
			  filesel->fileop_c_dir, TRUE, TRUE, 0);
      gtk_widget_show (filesel->fileop_c_dir);
    }
	
  if (!filesel->fileop_del_file) 
    {
      filesel->fileop_del_file = gtk_button_new_with_label (_("Delete File"));
      gtk_signal_connect (GTK_OBJECT (filesel->fileop_del_file), "clicked",
			  (GtkSignalFunc) gtk_file_selection_delete_file, 
			  (gpointer) filesel);
      gtk_box_pack_start (GTK_BOX (filesel->button_area), 
			  filesel->fileop_del_file, TRUE, TRUE, 0);
      gtk_widget_show (filesel->fileop_del_file);
    }

  if (!filesel->fileop_ren_file)
    {
      filesel->fileop_ren_file = gtk_button_new_with_label (_("Rename File"));
      gtk_signal_connect (GTK_OBJECT (filesel->fileop_ren_file), "clicked",
			  (GtkSignalFunc) gtk_file_selection_rename_file, 
			  (gpointer) filesel);
      gtk_box_pack_start (GTK_BOX (filesel->button_area), 
			  filesel->fileop_ren_file, TRUE, TRUE, 0);
      gtk_widget_show (filesel->fileop_ren_file);
    }

  gtk_widget_queue_resize(GTK_WIDGET(filesel));
}

void       
gtk_file_selection_hide_fileop_buttons (GtkFileSelection *filesel)
{
  g_return_if_fail (filesel != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (filesel));
    
  if (filesel->fileop_ren_file) 
    {
      gtk_widget_destroy (filesel->fileop_ren_file);
      filesel->fileop_ren_file = NULL;
    }

  if (filesel->fileop_del_file)
    {
      gtk_widget_destroy (filesel->fileop_del_file);
      filesel->fileop_del_file = NULL;
    }

  if (filesel->fileop_c_dir)
    {
      gtk_widget_destroy (filesel->fileop_c_dir);
      filesel->fileop_c_dir = NULL;
    }
}



void
gtk_file_selection_set_filename (GtkFileSelection *filesel,
				 const gchar      *filename)
{
  char  buf[MAXPATHLEN];
  const char *name, *last_slash;

  g_return_if_fail (filesel != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (filesel));
  g_return_if_fail (filename != NULL);

  last_slash = strrchr (filename, '/');

  if (!last_slash)
    {
      buf[0] = 0;
      name = filename;
    }
  else
    {
      gint len = MIN (MAXPATHLEN - 1, last_slash - filename + 1);

      strncpy (buf, filename, len);
      buf[len] = 0;

      name = last_slash + 1;
    }

  gtk_file_selection_populate (filesel, buf, FALSE, TRUE);

  if (filesel->selection_entry)
    gtk_entry_set_text (GTK_ENTRY (filesel->selection_entry), name);
}

gchar*
gtk_file_selection_get_filename (GtkFileSelection *filesel)
{
  static char nothing[2] = "";
  char *filename;

  g_return_val_if_fail (filesel != NULL, nothing);
  g_return_val_if_fail (GTK_IS_FILE_SELECTION (filesel), nothing);

  text = gtk_entry_get_text (GTK_ENTRY (filesel->selection_entry));
  if (text)
    {
      filename = cmpl_completion_fullname (text, filesel->cmpl_state);
      return filename;
    }

  return nothing;
}

void
gtk_file_selection_complete (GtkFileSelection *filesel,
			     const gchar      *pattern)
{
  g_return_if_fail (filesel != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (filesel));
  g_return_if_fail (pattern != NULL);

  if (filesel->selection_entry)
    gtk_entry_set_text (GTK_ENTRY (filesel->selection_entry), pattern);
  gtk_file_selection_populate (filesel, (gchar*) pattern, TRUE, TRUE);
}

static void
gtk_file_selection_destroy (GtkObject *object)
{
  GtkFileSelection *filesel;
  GList *list;
  HistoryCallbackArg *callback_arg;

  g_return_if_fail (object != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (object));

  filesel = GTK_FILE_SELECTION (object);
  
  if (filesel->fileop_dialog)
	  gtk_widget_destroy (filesel->fileop_dialog);
  
  if (filesel->history_list)
    {
      list = filesel->history_list;
      while (list)
	{
	  callback_arg = list->data;
	  g_free (callback_arg->directory);
	  g_free (callback_arg);
	  list = list->next;
	}
      g_list_free (filesel->history_list);
      filesel->history_list = NULL;
    }
  
  cmpl_free_state (filesel->cmpl_state);
  filesel->cmpl_state = NULL;

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (object);
}

/* Begin file operations callbacks */

static void
gtk_file_selection_fileop_error (GtkFileSelection *fs, gchar *error_message)
{
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *button;
  GtkWidget *dialog;
  
  g_return_if_fail (error_message != NULL);
  
  /* main dialog */
  dialog = gtk_dialog_new ();
  /*
  gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
		      (GtkSignalFunc) gtk_file_selection_fileop_destroy, 
		      (gpointer) fs);
  */
  gtk_window_set_title (GTK_WINDOW (dialog), _("Error"));
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  
  /* If file dialog is grabbed, make this dialog modal too */
  /* When error dialog is closed, file dialog will be grabbed again */
  if (GTK_WINDOW(fs)->modal)
      gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox,
		     FALSE, FALSE, 0);
  gtk_widget_show(vbox);

  label = gtk_label_new(error_message);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
  gtk_widget_show(label);

  /* yes, we free it */
  g_free (error_message);
  
  /* close button */
  button = gtk_button_new_with_label (_("Close"));
  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     (GtkSignalFunc) gtk_widget_destroy, 
			     (gpointer) dialog);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
		     button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button);
  gtk_widget_show (button);

  gtk_widget_show (dialog);
}

static void
gtk_file_selection_fileop_destroy (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;

  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));
  
  fs->fileop_dialog = NULL;
}


static void
gtk_file_selection_create_dir_confirmed (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;
  gchar *dirname;
  gchar *path;
  gchar *full_path;
  gchar *buf;
  CompletionState *cmpl_state;
  
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  dirname = gtk_entry_get_text (GTK_ENTRY (fs->fileop_entry));
  cmpl_state = (CompletionState*) fs->cmpl_state;
  path = cmpl_reference_position (cmpl_state);
  
  full_path = g_strconcat (path, "/", dirname, NULL);
  if ( (mkdir (full_path, 0755) < 0) ) 
    {
      buf = g_strconcat ("Error creating directory \"", dirname, "\":  ", 
			 g_strerror(errno), NULL);
      gtk_file_selection_fileop_error (fs, buf);
    }
  g_free (full_path);
  
  gtk_widget_destroy (fs->fileop_dialog);
  gtk_file_selection_populate (fs, "", FALSE, FALSE);
}
  
static void
gtk_file_selection_create_dir (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;
  GtkWidget *label;
  GtkWidget *dialog;
  GtkWidget *vbox;
  GtkWidget *button;

  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  if (fs->fileop_dialog)
	  return;
  
  /* main dialog */
  fs->fileop_dialog = dialog = gtk_dialog_new ();
  gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
		      (GtkSignalFunc) gtk_file_selection_fileop_destroy, 
		      (gpointer) fs);
  gtk_window_set_title (GTK_WINDOW (dialog), _("Create Directory"));
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

  /* If file dialog is grabbed, grab option dialog */
  /* When option dialog is closed, file dialog will be grabbed again */
  if (GTK_WINDOW(fs)->modal)
      gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox,
		     FALSE, FALSE, 0);
  gtk_widget_show(vbox);
  
  label = gtk_label_new(_("Directory name:"));
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
  gtk_widget_show(label);

  /*  The directory entry widget  */
  fs->fileop_entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (vbox), fs->fileop_entry, 
		      TRUE, TRUE, 5);
  GTK_WIDGET_SET_FLAGS(fs->fileop_entry, GTK_CAN_DEFAULT);
  gtk_widget_show (fs->fileop_entry);
  
  /* buttons */
  button = gtk_button_new_with_label (_("Create"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      (GtkSignalFunc) gtk_file_selection_create_dir_confirmed, 
		      (gpointer) fs);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
		     button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_show(button);
  
  button = gtk_button_new_with_label (_("Cancel"));
  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     (GtkSignalFunc) gtk_widget_destroy, 
			     (gpointer) dialog);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
		     button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button);
  gtk_widget_show (button);

  gtk_widget_show (dialog);
}

static void
gtk_file_selection_delete_file_confirmed (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;
  CompletionState *cmpl_state;
  gchar *path;
  gchar *full_path;
  gchar *buf;
  
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  cmpl_state = (CompletionState*) fs->cmpl_state;
  path = cmpl_reference_position (cmpl_state);
  
  full_path = g_strconcat (path, "/", fs->fileop_file, NULL);
  if ( (unlink (full_path) < 0) ) 
    {
      buf = g_strconcat ("Error deleting file \"", fs->fileop_file, "\":  ", 
			 g_strerror(errno), NULL);
      gtk_file_selection_fileop_error (fs, buf);
    }
  g_free (full_path);
  
  gtk_widget_destroy (fs->fileop_dialog);
  gtk_file_selection_populate (fs, "", FALSE, TRUE);
}

static void
gtk_file_selection_delete_file (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *button;
  GtkWidget *dialog;
  gchar *filename;
  gchar *buf;
  
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  if (fs->fileop_dialog)
	  return;

  filename = gtk_entry_get_text (GTK_ENTRY (fs->selection_entry));
  if (strlen(filename) < 1)
	  return;

  fs->fileop_file = filename;
  
  /* main dialog */
  fs->fileop_dialog = dialog = gtk_dialog_new ();
  gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
		      (GtkSignalFunc) gtk_file_selection_fileop_destroy, 
		      (gpointer) fs);
  gtk_window_set_title (GTK_WINDOW (dialog), _("Delete File"));
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

  /* If file dialog is grabbed, grab option dialog */
  /* When option dialog is closed, file dialog will be grabbed again */
  if (GTK_WINDOW(fs)->modal)
      gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);
  
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox,
		     FALSE, FALSE, 0);
  gtk_widget_show(vbox);

  buf = g_strconcat ("Really delete file \"", filename, "\" ?", NULL);
  label = gtk_label_new(buf);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
  gtk_widget_show(label);
  g_free(buf);
  
  /* buttons */
  button = gtk_button_new_with_label (_("Delete"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      (GtkSignalFunc) gtk_file_selection_delete_file_confirmed, 
		      (gpointer) fs);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
		     button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_show(button);
  
  button = gtk_button_new_with_label (_("Cancel"));
  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     (GtkSignalFunc) gtk_widget_destroy, 
			     (gpointer) dialog);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
		     button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button);
  gtk_widget_show (button);

  gtk_widget_show (dialog);

}

static void
gtk_file_selection_rename_file_confirmed (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;
  gchar *buf;
  gchar *file;
  gchar *path;
  gchar *new_filename;
  gchar *old_filename;
  CompletionState *cmpl_state;
  
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  file = gtk_entry_get_text (GTK_ENTRY (fs->fileop_entry));
  cmpl_state = (CompletionState*) fs->cmpl_state;
  path = cmpl_reference_position (cmpl_state);
  
  new_filename = g_strconcat (path, "/", file, NULL);
  old_filename = g_strconcat (path, "/", fs->fileop_file, NULL);

  if ( (rename (old_filename, new_filename)) < 0) 
    {
      buf = g_strconcat ("Error renaming file \"", file, "\":  ", 
			 g_strerror(errno), NULL);
      gtk_file_selection_fileop_error (fs, buf);
    }
  g_free (new_filename);
  g_free (old_filename);
  
  gtk_file_selection_populate (fs, "", FALSE, FALSE);
  gtk_entry_set_text (GTK_ENTRY (fs->selection_entry), file);
  gtk_widget_destroy (fs->fileop_dialog);
}
  
static void
gtk_file_selection_rename_file (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;
  GtkWidget *label;
  GtkWidget *dialog;
  GtkWidget *vbox;
  GtkWidget *button;
  gchar *buf;
  
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  if (fs->fileop_dialog)
	  return;

  fs->fileop_file = gtk_entry_get_text (GTK_ENTRY (fs->selection_entry));
  if (strlen(fs->fileop_file) < 1)
	  return;
  
  /* main dialog */
  fs->fileop_dialog = dialog = gtk_dialog_new ();
  gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
		      (GtkSignalFunc) gtk_file_selection_fileop_destroy, 
		      (gpointer) fs);
  gtk_window_set_title (GTK_WINDOW (dialog), _("Rename File"));
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

  /* If file dialog is grabbed, grab option dialog */
  /* When option dialog  closed, file dialog will be grabbed again */
  if (GTK_WINDOW(fs)->modal)
    gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);
  
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER(vbox), 8);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox,
		     FALSE, FALSE, 0);
  gtk_widget_show(vbox);
  
  buf = g_strconcat ("Rename file \"", fs->fileop_file, "\" to:", NULL);
  label = gtk_label_new(buf);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
  gtk_widget_show(label);
  g_free(buf);

  /* New filename entry */
  fs->fileop_entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (vbox), fs->fileop_entry, 
		      TRUE, TRUE, 5);
  GTK_WIDGET_SET_FLAGS(fs->fileop_entry, GTK_CAN_DEFAULT);
  gtk_widget_show (fs->fileop_entry);
  
  gtk_entry_set_text (GTK_ENTRY (fs->fileop_entry), fs->fileop_file);
  gtk_editable_select_region (GTK_EDITABLE (fs->fileop_entry),
			      0, strlen (fs->fileop_file));

  /* buttons */
  button = gtk_button_new_with_label (_("Rename"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      (GtkSignalFunc) gtk_file_selection_rename_file_confirmed, 
		      (gpointer) fs);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
		     button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_show(button);
  
  button = gtk_button_new_with_label (_("Cancel"));
  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     (GtkSignalFunc) gtk_widget_destroy, 
			     (gpointer) dialog);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
		     button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button);
  gtk_widget_show (button);

  gtk_widget_show (dialog);
}


static void
home_clicked (GtkWidget *widget, gpointer data)
{
  char *dir;
  
  dir = g_strdup_printf ("%s/", g_get_home_dir());
  
  gtk_file_selection_populate (GTK_FILE_SELECTION (data), 
			       dir, FALSE, TRUE);
  g_free (dir);
}

static void
desktop_clicked (GtkWidget *widget, gpointer data)
{
  char *dir;
  
  dir = g_strdup_printf ("%s/.gnome-desktop/", g_get_home_dir ());
  
  gtk_file_selection_populate (GTK_FILE_SELECTION (data), 
			       dir, FALSE, TRUE);
  g_free (dir);
}

static void 
documents_clicked (GtkWidget *widget, gpointer data)
{
  char *dir;
  dir = g_strdup_printf ("%s/Documents/", g_get_home_dir ());
  
  gtk_file_selection_populate (GTK_FILE_SELECTION (data), dir, FALSE, TRUE);
  
  g_free (dir);
}

static gint
gtk_file_selection_key_press (GtkWidget   *widget,
			      GdkEventKey *event,
			      gpointer     user_data)
{
  GtkFileSelection *fs;
  char *text;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  fs = GTK_FILE_SELECTION (user_data);
  gtk_object_set_data (GTK_OBJECT (fs->selection_entry), "last_entry_was_dir_selection", (gpointer) FALSE);

  if (event->keyval == GDK_Tab)
    {
      text = gtk_entry_get_text (GTK_ENTRY (fs->selection_entry));

      text = g_strdup (text);

      gtk_file_selection_populate (fs, text, TRUE, TRUE);

      g_free (text);

      gtk_signal_emit_stop_by_name (GTK_OBJECT (widget), "key_press_event");

      return TRUE;
    }

  return FALSE;
}

static void
gtk_file_selection_history_callback (GtkWidget *widget, gpointer data)
{
  GtkFileSelection *fs = data;
  HistoryCallbackArg *callback_arg;
  GList *list;

  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  list = fs->history_list;
  
  while (list) {
    callback_arg = list->data;
    
    if (callback_arg->menu_item == widget)
      {
	gtk_file_selection_populate (fs, callback_arg->directory, FALSE, FALSE);
	break;
      }
    
    list = list->next;
  }
}

static void 
gtk_file_selection_update_history_menu (GtkFileSelection *fs,
					gchar *current_directory)
{
  HistoryCallbackArg *callback_arg;
  GtkWidget *menu_item;
  GList *list;
  gchar *current_dir;
  gint dir_len;
  gint i;
  
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));
  g_return_if_fail (current_directory != NULL);
  
  list = fs->history_list;

  if (fs->history_menu) 
    {
      while (list) {
	callback_arg = list->data;
	g_free (callback_arg->directory);
	g_free (callback_arg);
	list = list->next;
      }
      g_list_free (fs->history_list);
      fs->history_list = NULL;
      
      gtk_widget_destroy (fs->history_menu);
    }
  
  fs->history_menu = gtk_menu_new();

  current_dir = g_strdup (current_directory);

  dir_len = strlen (current_dir);

  for (i = dir_len; i >= 0; i--)
    {
      /* the i == dir_len is to catch the full path for the first 
       * entry. */
      if ( (current_dir[i] == '/') || (i == dir_len))
	{
	  /* another small hack to catch the full path */
	  if (i != dir_len) 
		  current_dir[i + 1] = '\0';
	  menu_item = gtk_menu_item_new_with_label (current_dir);
	  
	  callback_arg = g_new (HistoryCallbackArg, 1);
	  callback_arg->menu_item = menu_item;
	  
	  /* since the autocompletion gets confused if you don't 
	   * supply a trailing '/' on a dir entry, set the full
	   * (current) path to "" which just refreshes the filesel */
	  if (dir_len == i) {
	    callback_arg->directory = g_strdup ("");
	  } else {
	    callback_arg->directory = g_strdup (current_dir);
	  }
	  
	  fs->history_list = g_list_append (fs->history_list, callback_arg);
	  
	  gtk_signal_connect (GTK_OBJECT (menu_item), "activate",
			      (GtkSignalFunc) gtk_file_selection_history_callback,
			      (gpointer) fs);
	  gtk_menu_append (GTK_MENU (fs->history_menu), menu_item);
	  gtk_widget_show (menu_item);
	}
    }

  gtk_option_menu_set_menu (GTK_OPTION_MENU (fs->history_pulldown), 
			    fs->history_menu);
  g_free (current_dir);
}

static void
gtk_file_selection_file_button (GtkWidget *widget,
			       gint row, 
			       gint column, 
			       GdkEventButton *bevent,
			       gpointer user_data)
{
  GtkFileSelection *fs = NULL;
  gchar *filename, *temp = NULL;
  const gchar *entry_text;
  gboolean last_entry_was_dir_selection;
  
  g_return_if_fail (GTK_IS_CLIST (widget));

  fs = user_data;
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));
  
  gtk_clist_get_text (GTK_CLIST (fs->file_list), row, 0, &temp);
  filename = g_strdup (temp);
  
  entry_text = gtk_entry_get_text (GTK_ENTRY (fs->selection_entry));
   last_entry_was_dir_selection = (gboolean) gtk_object_get_data (GTK_OBJECT (fs->selection_entry), "last_entry_was_dir_selection");

  if (filename)
    {
      gtk_object_set_data (GTK_OBJECT (fs->selection_entry), "last_entry_was_dir_selection", (gpointer) FALSE);
			if (bevent)
	switch (bevent->type)
	  {
	  case GDK_2BUTTON_PRESS:
	    gtk_button_clicked (GTK_BUTTON (fs->ok_button));
	    break;
	    
	  default:
	    if (!entry_text || !strlen (entry_text) || last_entry_was_dir_selection)
	      {
		gtk_entry_set_text (GTK_ENTRY (fs->selection_entry), filename);
		last_entry_was_dir_selection = TRUE;
	      }
	    break;
	  }
      else if (!entry_text || !strlen (entry_text) || last_entry_was_dir_selection)
	  {
	    gtk_entry_set_text (GTK_ENTRY (fs->selection_entry), filename);
	    last_entry_was_dir_selection = TRUE;
	  }

      g_free (filename);
    }

  gtk_object_set_data (GTK_OBJECT (fs->selection_entry), "last_entry_was_dir_selection",
		       (gpointer) last_entry_was_dir_selection);
}

static void
gtk_file_selection_dir_button (GtkWidget *widget,
			       gint row, 
			       gint column, 
			       GdkEventButton *bevent,
			       gpointer user_data)
{
  GtkFileSelection *fs = NULL;
  gchar *filename = NULL;

  g_return_if_fail (GTK_IS_CLIST (widget));

  fs = GTK_FILE_SELECTION (user_data);
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  gtk_clist_get_text (GTK_CLIST (fs->dir_list), row, 0, &filename);
  
  if (filename && bevent && bevent->type == GDK_2BUTTON_PRESS)
    gtk_file_selection_populate (fs, filename, FALSE, FALSE);
}


/**********************************************************************/
/*                        The main entrances.                         */
/**********************************************************************/

static PossibleCompletion* cmpl_next_completion (CompletionState* cmpl_state)
{
  PossibleCompletion* poss = NULL;

  cmpl_state->the_completion.text[0] = 0;

  if(cmpl_state->user_completion_index >= 0)
    poss = attempt_homedir_completion(cmpl_state->last_completion_text, cmpl_state);
  else
    poss = attempt_file_completion(cmpl_state);

  update_cmpl(poss, cmpl_state);

  return poss;
}

static void gtk_file_selection_populate (GtkFileSelection *fs,
			     gchar            *rel_path,
			     gboolean          try_complete,
			     gboolean          reset_entry)
{
  CompletionState *cmpl_state;
  PossibleCompletion* poss;
  gchar* filename;
  gint row;
  gchar* rem_path;
  gchar* sel_text;
  gchar* text[2];
  gint did_recurse = FALSE;
  gint possible_count = 0;
  gint selection_index = -1;
  gint file_list_width;
  gint dir_list_width;
  
  g_return_if_fail (fs != NULL);
  g_return_if_fail (GTK_IS_FILE_SELECTION (fs));

  rem_path = rel_path = g_strdup (rel_path);
  
  cmpl_state = (CompletionState*) fs->cmpl_state;
  poss = cmpl_completion_matches (rel_path, &rem_path, cmpl_state);

  if (!cmpl_state_okay (cmpl_state))
    {
      /* Something went wrong. */
      gtk_file_selection_abort (fs);
      return;
    }

  g_assert (cmpl_state->reference_dir);

  gtk_clist_freeze (GTK_CLIST (fs->dir_list));
  gtk_clist_clear (GTK_CLIST (fs->dir_list));
  gtk_clist_freeze (GTK_CLIST (fs->file_list));
  gtk_clist_clear (GTK_CLIST (fs->file_list));

  /* Set the dir_list to include ./ and ../ */
  text[1] = NULL;
  text[0] = "./";
  row = gtk_clist_append (GTK_CLIST (fs->dir_list), text);

  text[0] = "../";
  row = gtk_clist_append (GTK_CLIST (fs->dir_list), text);

  /*reset the max widths of the lists*/
  dir_list_width = gdk_string_width(fs->dir_list->style->font,"../");
  gtk_clist_set_column_width(GTK_CLIST(fs->dir_list),0,dir_list_width);
  file_list_width = 1;
  gtk_clist_set_column_width(GTK_CLIST(fs->file_list),0,file_list_width);

  while (poss)
    {
      if (cmpl_is_a_completion (poss))
        {
          possible_count += 1;

          filename = cmpl_this_completion (poss);

	  text[0] = filename;
	  
          if (cmpl_is_directory (poss))
            {
              if (strcmp (filename, "./") != 0 &&
                  strcmp (filename, "../") != 0)
		{
		  int width = gdk_string_width(fs->dir_list->style->font,
					       filename);
		  row = gtk_clist_append (GTK_CLIST (fs->dir_list), text);
		  if(width > dir_list_width)
		    {
		      dir_list_width = width;
		      gtk_clist_set_column_width(GTK_CLIST(fs->dir_list),0,
						 width);
		    }
 		}
	    }
          else
	    {
	      int width = gdk_string_width(fs->file_list->style->font,
				           filename);
	      row = gtk_clist_append (GTK_CLIST (fs->file_list), text);
	      if(width > file_list_width)
	        {
	          file_list_width = width;
	          gtk_clist_set_column_width(GTK_CLIST(fs->file_list),0,
					     width);
	        }
            }
	}

      poss = cmpl_next_completion (cmpl_state);
    }

  gtk_clist_thaw (GTK_CLIST (fs->dir_list));
  gtk_clist_thaw (GTK_CLIST (fs->file_list));

  /* File lists are set. */

  g_assert (cmpl_state->reference_dir);

  if (try_complete)
    {

      /* User is trying to complete filenames, so advance the user's input
       * string to the updated_text, which is the common leading substring
       * of all possible completions, and if its a directory attempt
       * attempt completions in it. */

      if (cmpl_updated_text (cmpl_state)[0])
        {

          if (cmpl_updated_dir (cmpl_state))
            {
	      gchar* dir_name = g_strdup (cmpl_updated_text (cmpl_state));

              did_recurse = TRUE;

              gtk_file_selection_populate (fs, dir_name, TRUE, TRUE);

              g_free (dir_name);
            }
          else
            {
	      if (fs->selection_entry)
		      gtk_entry_set_text (GTK_ENTRY (fs->selection_entry),
					  cmpl_updated_text (cmpl_state));
            }
        }
      else
        {
          selection_index = cmpl_last_valid_char (cmpl_state) -
                            (strlen (rel_path) - strlen (rem_path));
	  if (fs->selection_entry)
	    gtk_entry_set_text (GTK_ENTRY (fs->selection_entry), rem_path);
        }
    }
  else if (reset_entry)
    {
      gboolean last_entry_was_dir_selection;

      last_entry_was_dir_selection = (gboolean) gtk_object_get_data (GTK_OBJECT (fs->selection_entry),
								     "last_entry_was_dir_selection");

      if (fs->selection_entry && last_entry_was_dir_selection)
	gtk_entry_set_text (GTK_ENTRY (fs->selection_entry), "");
    }

  if (!did_recurse)
    {
      char *dirname;
      struct stat ent_sbuf;
      if (fs->selection_entry)
	gtk_entry_set_position (GTK_ENTRY (fs->selection_entry), selection_index);

      if (fs->selection_entry)
	{
	  sel_text = g_strconcat (_("Selection: "),
				  cmpl_reference_position (cmpl_state),
				  NULL);

	  gtk_label_set_text (GTK_LABEL (fs->selection_text), sel_text);
	  g_free (sel_text);
	}

      if (fs->history_pulldown) 
	{
	  gtk_file_selection_update_history_menu (fs, cmpl_reference_position (cmpl_state));
	}
	
	dirname = cmpl_reference_position (cmpl_state);
        if(dirname && stat(dirname, &ent_sbuf) >= 0 && S_ISDIR(ent_sbuf.st_mode))
          {
            if (last_dir) 
              {
                g_free (last_dir);
              }
  
  
            
            last_dir = g_strdup_printf ("%s%c", dirname, G_DIR_SEPARATOR);
    }

  g_free (rel_path);
}}
