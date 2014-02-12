toplevel .ex1
wm geometry .ex1 730x660+250+100
wm title .ex1 ""
#wm transient .ex1
#wm overrideredirect .ex1 1

frame .ex1.fr1 -bg blue
text .ex1.c -width 80 -height 18 -bg #aaffff -bd 3 -font 9x15\
   -highlightthickness 5 -highlightbackground #aa00aa -highlightcolor #bb00bb
pack .ex1.c -side top -fill both -expand true
pack .ex1.fr1 -side top -fill both -expand false
button .ex1.fr1.dismiss -bg #ff88ee -activebackground #aaffff\
   -text "Dismiss" -command "destroy .ex1"
pack .ex1.fr1.dismiss -pady 10

.ex1.c insert 1.0 "Illustration of Polynomial Formats"
.ex1.c tag add title 1.0 1.end

.ex1.c tag configure title -justify center -foreground #aa0000\
   -font -adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-*-*
   
for {set i 1} {$i <= 15} {incr i} {
   .ex1.c insert  insert "\n"
}

.ex1.c insert 3.0 "Knot:    5  1     4  8 10  2  6"
.ex1.c tag add heading 3.0 3.end

.ex1.c tag configure heading -justify center\
   -font 10x20
   
.ex1.c insert 7.0 "Jones Polynomial:   1  6     1 -1  2 -1  1 -1"
.ex1.c insert 7.end "      t - t2 + 2t3 - t4 + t5 - t6"

foreach item {56 62 67 72 77} {
   .ex1.c tag add superscript 7.$item 
}

.ex1.c insert 11.end "
Homfly Polynomial:  2  6     1  1 -1           (v2 + v4 - v6) z0 +

                    2  4     1  1              (v2 + v4) z2"

foreach item {49 54 59 63} {
   .ex1.c tag add superscript 12.$item 
}

foreach item {49 54 58} {
   .ex1.c tag add superscript 14.$item 
}

.ex1.c tag configure superscript -offset 10 -font 6x13

set polyVersionPic [image create photo -file $ks_dir/gif/polyversion.gif]

.ex1.c window create 18.0\
  -create "label .ex1.c.lab -image $polyVersionPic" -padx 70



update idletasks
#grab set .ex1
#tkwait window .ex1

