global ks_dir tmp_dir home_dir

set ks_dir $env(KSHOME)
set tmp_dir $env(TMPDIR)
set home_dir $env(HOME)

#load default colors and fonts

source $ks_dir/tcl/knotscape_options.tcl

#load procedures

source $ks_dir/tcl/filedialog.tcl
source $ks_dir/tcl/knotscape_procs.tcl
source $ks_dir/tcl/linksmith_procs.tcl

wm withdraw .

#toplevel .prolog
#set picture1 [image create photo -format GIF -file $ks_dir/gif/knotvig.gif]
#set picture1 [image create photo -file $ks_dir/gif/torus.gif]
#set picture2 [image create photo -file $ks_dir/gif/title.gif]
#label .prolog.lbl1 -image $picture1
#label .prolog.lbl2 -image $picture2 -bg black
#pack .prolog.lbl1 .prolog.lbl2 -fill both -expand true
#raise .prolog
#wm geometry .prolog +50+50
#update idletasks
#after 1000
#destroy .prolog
 
#create main windows

showInputWindow "firstTime"
wm protocol .input WM_DELETE_WINDOW {
   wm withdraw .input
} 

showOutputWindow "firstTime"
wm protocol .output WM_DELETE_WINDOW {
   wm withdraw .output
} 

showPolyChoice "firstTime"  
wm protocol .polynomials WM_DELETE_WINDOW {
   wm withdraw .polynomials
} 

showTableChoice "firstTime"  
wm protocol .tableChoice WM_DELETE_WINDOW {
   wm withdraw .tableChoice
} 

showHomChoice "firstTime"  
wm protocol .homChoice WM_DELETE_WINDOW {
   wm withdraw .homChoice
}

showLinkSmith "firstTime"
wm protocol .linksmith WM_DELETE_WINDOW {
   wm withdraw .linksmith
}

#display input window

showInputWindow x

#display introductory message

#frame .options -bg blue

#set picture3 [image create photo -file $ks_dir/gif/intro2.gif]

#label .intro -image $picture3
   
#button .options.dismiss -text Dismiss\
#   -bg #cc33cc -activebackground #ff55ff\
#   -command "wm withdraw ."
                            
#pack .intro .options -fill both -expand true
#pack .options.dismiss -padx 20 -pady 5 -side left
#update idletasks

#wm geometry . 470x450+50+120
wm geometry .input 1000x500+20+20

#wm deiconify .
#raise .

