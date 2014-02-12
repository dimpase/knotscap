toplevel .ex0
wm geometry .ex0 500x550+300+150
wm title .ex0 ""
#wm transient .ex0
#wm overrideredirect .ex0 1

frame .ex0.fr1 -bg blue
canvas .ex0.c -height 500 -width 500 -bg white
pack .ex0.c -side top
pack .ex0.fr1 -side top -fill both -expand true
button .ex0.fr1.dismiss -bg #ff88ee -activebackground #aaffff\
   -text "Dismiss" -command "destroy .ex0"
pack .ex0.fr1.dismiss -pady 10

.ex0.c create line 0 0 500 0 500 500 0 500 0 0 -fill #aa00aa\
   -width 10 -smooth 0
   
.ex0.c create text 250 20 -justify c\
   -font -adobe-helvetica-medium-r-*-*-20-*-*-*-*-*-*-*\
   -text "Illustration of Dowker Code"

.ex0.c create polygon 200 100 250  50 300 100 250 200 200 250 250 300\
                  400 300 400 200 300 100 200 100 100 200 100 300 250 300\
                  300 250 250 200 -smooth 1 -fill white -outline blue\
                  -width 3
                  
.ex0.c create oval 201 104 211 114 -fill white -outline white
.ex0.c create oval 215 101 225 111 -fill white -outline white
.ex0.c create oval 282.6 95.3 292.6 105.3 -fill white -outline white
.ex0.c create oval 280.8 109.2 290.8 119.2 -fill white -outline white
.ex0.c create oval 249.8 183.7 259.8 193.7 -fill white -outline white
.ex0.c create oval 240.3 194.2 250.3 204.2 -fill white -outline white
.ex0.c create oval 249.8 279.4 259.8 289.4 -fill white -outline white
.ex0.c create oval 239.3 289.4 249.3 299.4 -fill white -outline white

.ex0.c create oval 207.5 102.0 217.5 112.0 -fill red
.ex0.c create oval 280.5 102.0 290.5 112.0 -fill red
.ex0.c create oval 244.5 189.0 254.5 199.0 -fill red
.ex0.c create oval 244.5 284.0 254.5 294.0 -fill red

.ex0.c create text 233.0 189.0 -font 9x15bold -text 1
.ex0.c create text 203.0 96.0 -font 9x15bold -text 2
.ex0.c create text 293.0 88.0 -font 9x15bold -text 3
.ex0.c create text 268.0 180.0 -font 9x15bold -text 4
.ex0.c create text 241.0 276.0 -font 9x15bold -text 5
.ex0.c create text 307.0 105.0 -font 9x15bold -text 6
.ex0.c create text 233.0 112.0 -font 9x15bold -text 7
.ex0.c create text 230.0 305.0 -font 9x15bold -text 8

.ex0.c create text 200 400 -font 10x20 -text "1   3   5   7"
.ex0.c create text 200 440 -font 10x20 -text "4  -6   8  -2"

.ex0.c create rectangle 120.0 379.0 278.0 458.0
.ex0.c create rectangle 129.0 425.0 270.0 451.0 -outline red -width 2


update idletasks
#grab set .ex0
#tkwait window .ex0

