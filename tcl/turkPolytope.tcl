toplevel .polytope
wm geometry .polytope 500x550+250+100
wm title .polytope "Turk's Head Polytope"

set turkGif [image create photo -file $ks_dir/gif/turk_prism.gif]

frame .polytope.fr -bg blue
label .polytope.lab -height 500 -width 500 -bg black -image $turkGif -highlightthickness 0\
  -highlightcolor red

pack .polytope.lab -side top -fill both -expand false
pack .polytope.fr -side top -fill both -expand false
button .polytope.fr.dismiss -bg #ff88ee -activebackground #aaffff\
   -text "Dismiss" -command "destroy .polytope"
pack .polytope.fr.dismiss -pady 10


update idletasks
#grab set .polytope
#tkwait window .polytope

