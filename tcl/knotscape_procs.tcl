############################################################################
#                               input window                               #
############################################################################

proc showInputWindow {init} {
   if { $init=="firstTime"} {
      toplevel .input
      wm withdraw .input
      wm geometry .input 800x500+20+20
      wm title .input "Input Window"
      
#suppress default binding for radiobuttons

      bind Radiobutton <Return> {}
      
#initialize global variables used in stats bar
      
      global ks_dir tmp_dir zoomExists
      global inputFilename cursorLineNumber cursorColumnNumber nBytes
      
      set inputFilename "None"
      set cursorLineNumber 1
      set cursorColumnNumber 0
      set nBytes 0

#assemble main constituents of input window
  
      frame .input.top -relief raised -bd 3 -bg #85bdff
      frame .input.top.mbar -bg #85bdff
      frame .input.stat -bg wheat
      frame .input.fr1
      
      pack .input.top -side top -fill both -expand true
      pack .input.top.mbar -side left -fill y -anchor w
      pack .input.top -side top -fill both -expand false
      pack .input.stat -side top -fill both -expand false
      pack .input.fr1 -side top -fill both -expand true
      
#set up stats bar
            
      foreach item {{1 "Current file: " inputFilename}\
                    {2 "      Line: " cursorLineNumber}\
                    {3 "      Column: " cursorColumnNumber}} {
         set a [lindex $item 0]
         set b [lindex $item 1]
         set c [lindex $item 2]

         label .input.stat.left$a -bg wheat -text $b
         label .input.stat.right$a -bg wheat -textvariable $c
         pack .input.stat.left$a .input.stat.right$a\
            -anchor w -side left -expand false
      }

#set up menu bar

      set fileGif [image create photo -file $ks_dir/gif/file.gif]
      set actionGif [image create photo -file $ks_dir/gif/action.gif]
      set browseGif [image create photo -file $ks_dir/gif/browse.gif]
      set helpGif [image create photo -file $ks_dir/gif/help.gif]
        
      menubutton .input.top.mbar.file -image $fileGif\
         -menu .input.top.mbar.file.menu -width 99 -height 32\
         -relief raised -bd 2 -bg #a88bff -activebackground #00e0ff
      menu .input.top.mbar.file.menu -tearoff false
      pack .input.top.mbar.file -side left -padx 20 -pady 5 

      menubutton .input.top.mbar.action -image $actionGif\
         -menu .input.top.mbar.action.menu -width 99 -height 32\
         -relief raised -bd 2 -bg #a88bff -activebackground #00e0ff
      menu .input.top.mbar.action.menu -tearoff true\
         -background #d0d0ff -activebackground #00e0ff
      pack .input.top.mbar.action -side left -padx 20 -pady 5 

      foreach item {{Open "openFile .input" "  Ctrl-o"}
      {Close closeInput "  Ctrl-x"}
      {LinkSmith "showLinkSmith x" "  Ctrl-k"}
      {"Enter Braid" enterBraid "  Ctrl-b"}
      {Save "saveFile .input" "  Ctrl-s"}
      {"Save as .." "saveAs .input" "  Ctrl-S"}
      {Quit tidyExit "  Ctrl-Alt-q"}} {
         .input.top.mbar.file.menu add command -label [lindex $item 0]\
          -background #d0d0ff -activebackground #00e0ff -command [lindex $item 1]\
	  -accelerator [lindex $item 2]
      }

      bindtags .input [list %W all Text]
      
      bind .input <Control-o> {openFile .input}
      bind .input <Control-x> {closeInput}
      bind .input <Control-k> {showLinkSmith x}
      bind .input <Control-s> {saveFile .input}
      bind .input <Control-S> {saveAs .input}
      bind .input <Control-Alt-q> {tidyExit}

      foreach item {{"Locate in Table" "locateInTable" "  Ctrl-l"}
      {"Draw Knot" "drawKnot" "  Ctrl-d"}
      {"Make Ps" "psKnot" "  Ctrl-P"}
      {"Compute Polynomials" "showPolyChoice x" "  Ctrl-p"}
      {"Find Homomorphisms" "showHomChoice x" "  Ctrl-h"}
      {"Determinant & Signature" "detSigCompute" "  Ctrl-f"}} {
         .input.top.mbar.action.menu add command -label [lindex $item 0]\
          -background #d0d0ff -activebackground #00e0ff -command [lindex $item 1]\
	  -accelerator [lindex $item 2]
      }
      
      .input.top.mbar.action.menu add cascade -label "Hyperbolic Invariants"\
         -menu .input.top.mbar.action.menu.hyperbolic -background #d0d0ff\
	 -activebackground #00e0ff
      
      menu .input.top.mbar.action.menu.hyperbolic -tearoff true -background #d0d0ff\
         -activebackground #00e0ff
      
      foreach item {
      {"Volume" "hypInvariants 1" "  Ctrl-v"}
      {"Symmetry group" "hypInvariants 2" "  Ctrl-g"}
      {"Maximal Cusp Neighborhood" "hypInvariants 3" "  Ctrl-c"}
      {"Detailed Symmetry Information" "symmetryInfo" "  Ctrl-i"}
      {"Canonical Cell Decomposition" "canonicalPicture" "  Ctrl-C"}
      {"Save Triangulation" "saveKnotTriangulation" "  Ctrl-t"}} {
         .input.top.mbar.action.menu.hyperbolic add command -label [lindex $item 0]\
          -background #d0d0ff -activebackground #00e0ff -command [lindex $item 1]\
	  -accelerator [lindex $item 2]
      }
      
           
      .input.top.mbar.action.menu.hyperbolic add cascade -label "Horoball Diagram"\
         -menu .input.top.mbar.action.menu.hyperbolic.horoball -background #d0d0ff\
	 -activebackground #00e0ff
      
      menu .input.top.mbar.action.menu.hyperbolic.horoball -tearoff true -background #d0d0ff\
         -activebackground #00e0ff
	 
      .input.top.mbar.action.menu.hyperbolic.horoball add command -label "Cutoff diameter .5"\
         -command "horoballDiagram .5" -accelerator "   Ctrl-1"
      .input.top.mbar.action.menu.hyperbolic.horoball add command -label "Cutoff diameter .2"\
         -command "horoballDiagram .2" -accelerator "   Ctrl-2"
      .input.top.mbar.action.menu.hyperbolic.horoball add command -label "Cutoff diameter .1"\
         -command "horoballDiagram .1" -accelerator "   Ctrl-3"
      .input.top.mbar.action.menu.hyperbolic.horoball add command -label "Cutoff diameter .07"\
         -command "horoballDiagram .07" -accelerator "   Ctrl-4"
      .input.top.mbar.action.menu.hyperbolic.horoball add command -label "Cutoff diameter .05"\
         -command "horoballDiagram .05" -accelerator "   Ctrl-5"
      
      

      button .input.top.browse -image $browseGif -bg #00d090 -width 100 -height 30\
          -activebackground #30ffb0 -command "showTableChoice x"
       #   -font "-adobe-new century schoolbook-medium-r-*-*-20-*-*-*-*-*-*-*"
          
      pack .input.top.browse -side left -padx 20 -pady 5 
     
      
      button .input.top.help -image $helpGif -bg #00d090 -width 100 -height 30\
          -activebackground #30ffb0 -command "showHelp toc"
        #  -font "-adobe-new century schoolbook-medium-r-*-*-20-*-*-*-*-*-*-*"
          
      pack .input.top.help .input.top.browse -side left -padx 20 -pady 5 

#set up text widget     
  
      text .input.fr1.text\
         -yscrollcommand ".input.fr1.scroll set"
      scrollbar .input.fr1.scroll -command ".input.fr1.text yview" -bg blue\
         -activebackground #aa55ff -troughcolor #eebbee
      .input.fr1.text tag configure sel -relief raised -borderwidth 2
      
      bindtags .input.fr1.text [list .input %W all Text]
      
      bind .input <Control-l> {locateInTable}
      bind .input <Control-d> {drawKnot}
      bind .input <Control-v> {hypInvariants 1}
      bind .input <Control-g> {hypInvariants 2}
      bind .input <Control-c> {hypInvariants 3}
      bind .input <Control-i> {symmetryInfo}
      bind .input <Control-C> {canonicalPicture}
      bind .input <Control-t> {saveKnotTriangulation}
      bind .input <Control-p> {showPolyChoice x}
      bind .input <Control-h> {showHomChoice x}
      bind .input <Control-b> {enterBraid}
      bind .input <Control-f> {detSigCompute}
      bind .input <Control-KeyPress-1> "horoballDiagram .5"
      bind .input <Control-KeyPress-2> "horoballDiagram .2"
      bind .input <Control-KeyPress-3> "horoballDiagram .1"
      bind .input <Control-KeyPress-4> "horoballDiagram .07"
      bind .input <Control-KeyPress-5> "horoballDiagram .05"
      
      bind .input <Enter> "focus %W"
      
#institute bindings to render stats entries dynamic      

      bind .input <KeyPress> {
         scan [.input.fr1.text index insert] %%d.%%d cursorLineNumber cursorColumnNumber
      }
      
      bind .input <ButtonPress> {
         scan [.input.fr1.text index insert] %%d.%%d cursorLineNumber cursorColumnNumber
      }


      pack .input.fr1 -side top -fill both
      
      pack .input.fr1.scroll -side right -fill y
      pack .input.fr1.text -side left -fill both -expand true
      
      proc closeInput {} {
         global cursorLineNumber cursorColumnNumber nBytes inputFilename
         .input.fr1.text delete 1.0 end
         set inputFilename None
         set cursorLineNumber 1
         set cursorColumnNumber 0
         set nBytes 0
      }
           
   } else {
      wm deiconify .input
   }
}


#############################################################################
#                               Enter Braid                                 #
#############################################################################

proc enterBraid {} {
   
   global ks_dir tmp_dir braidWord
   
   toplevel .braidInput
   wm geometry .braidInput 750x180+400+400
   
   frame .braidInput.fr -bg #009900
   button .braidInput.fr.ok -bg #00cc00 -activebackground #aaffff\
   -highlightthickness 6 -highlightbackground #aaffaa\
    -text "OK" -command loadBraid
   button .braidInput.fr.dismiss -bg #00cc00 -activebackground #aaffff\
    -text "Cancel" -command "destroy .braidInput"
   label .braidInput.label -bg wheat -fg #333300\
    -text "Enter braid word as a sequence of alphabetic characters:\n\
     a, b, c, d denote standard braid generators, and A, B, C, D\
     denote their respective inverses.\n\
     For example aBaB denotes the 3-braid whose closure is the figure-eight knot."
   entry .braidInput.fr.entry -width 75 -bg #aaffaa -highlightbackground yellow\
     -textvariable braidWord 
   bind .braidInput <Return> loadBraid
      
   pack .braidInput.label -side top -fill both -expand true
   pack .braidInput.fr -fill both -expand true
   pack .braidInput.fr.entry -side top -pady 20
   pack .braidInput.fr.ok .braidInput.fr.dismiss -side left -pady 10 -expand true
   focus .braidInput.fr.entry
   update idletasks
   
}
   
proc loadBraid {} {
   
   global ks_dir tmp_dir braidWord
   
   if {$braidWord == {}} {
      return
   }
   
   set f [open $tmp_dir/K[pid]braid.in w]
  
   puts $f $braidWord

   close $f

   if {[catch {exec $ks_dir/bin/braid2dt $tmp_dir/K[pid]braid.in  $tmp_dir/K[pid]dt.out}] == 1} {
      warning "An error occurred in converting the braid word to DT form"
      return
   }
   
   set f [open $tmp_dir/K[pid]dt.out r]
   set str [gets $f]
   close $f
   
   if {[regexp {[a-z]} $str] == 0} {   
      writeToWindow .input 2 $tmp_dir/K[pid]dt.out
      destroy .braidInput
   } else {
      warning $str
   }
   
   update idletasks
}



#############################################################################
#                          Hyperbolic Invariants                            #
#############################################################################

proc hypInvariants {option} {
   
   global ks_dir tmp_dir
   
# read knot from selection, or read everything from input text widget if
# nothing is selected

  set f [open $tmp_dir/K[pid]hyp.in w]
  
  if {[string length [.input.fr1.text tag ranges sel]] == 0} {
    set knotData [.input.fr1.text get 1.0 end]
  } else {
    set knotData [.input.fr1.text get sel.first sel.last]
  }
  
  if {[checkIntegrity $knotData] == 1} {return}
  
  puts $f $knotData

  close $f
    
  update idletasks
  
  warning2 "Working ..." 400 80
  update idletasks
     
  if {[catch {exec $ks_dir/bin/knotscape_hyp $option $tmp_dir/K[pid]hyp.in  $tmp_dir/K[pid]hyp.out}] == 1} {
     destroy .warning2
     warning "An error has occurred in the computation of the hyperbolic invariant"
     return
  }

  writeToWindow .output 1 $tmp_dir/K[pid]hyp.out
  
  destroy .warning2
  update idletasks
}





#############################################################################
#                      Detailed Symmetry Information                        #
#############################################################################

proc symmetryInfo {} {
   
   global ks_dir tmp_dir
   
# read knot from selection, or from first 100 lines of input text widget if
# nothing is selected

  set f [open $tmp_dir/K[pid]hyp.in w]
  
  if {[string length [.input.fr1.text tag ranges sel]] == 0} {
    set knotData [.input.fr1.text get 1.0 end]
  } else {
    set knotData [.input.fr1.text get sel.first sel.last]
  }   
   
  puts $f $knotData

  close $f
  
  if {[checkIntegrity $knotData] == 1} {return}
  
  update idletasks
  
  warning2 "Working ..." 400 80
  update idletasks
     
  if {[catch {exec $ks_dir/bin/symmetry_fix_info $tmp_dir/K[pid]hyp.in  $tmp_dir/K[pid]hyp.out}] == 1} {
     destroy .warning2
     warning "An error has occurred in the computation of the fixed point set"
     return
  }
  
  writeToWindow .output 1 $tmp_dir/K[pid]hyp.out
  
  destroy .warning2
  update idletasks
}


#############################################################################
#                      Canonical Cell Decomposition                         #
#############################################################################


proc canonicalPicture {} {


  global ks_dir tmp_dir
  
  #  This procedure displays the intersection of the canonical cell
  #  decomposition with the horosphere z=1 .
  
  if {[winfo exists .canonicalPicture]} {
     warning "Multiple canonical decompositions not supported at present"
     return
  }


  #  read single knot from selection, or from line 1 of input text widget if
  #  nothing is selected

  set f [open $tmp_dir/K[pid]canonicalPic.in w]
  
  if {[string length [.input.fr1.text tag ranges sel]] == 0} {
    set knotData [.input.fr1.text get 1.0 1.end]
  } else {
    set selFirstIndex [.input.fr1.text index sel.first]
    scan $selFirstIndex "%d.%d" selFirstLine selFirstChar
    set selLastIndex [.input.fr1.text index sel.last]
    scan $selLastIndex "%d.%d" selLastLine selLastChar
    if {!($selFirstLine == $selLastLine ||\
         ([incr selFirstLine] == $selLastLine && $selLastChar == 0))} {
      warning "Please select single knot for canonical decomposition"
      return
    }
    
    set knotData [.input.fr1.text get sel.first sel.last]
    
  }
  
  puts $f $knotData

  close $f
  
  if {[checkIntegrity $knotData] == 1} {return}
  
  update idletasks
  
  warning2 "Computing ..." 400 80
  update idletasks
     
  if {[catch {exec $ks_dir/bin/knotscape_canonical $tmp_dir/K[pid]canonicalPic.in\
     $tmp_dir/K[pid]canonicalPic.out $tmp_dir/K[pid]canonical.ps}] == 1} {
     destroy .warning2
     warning "An error has occurred in the computation of the canonical decomposition"
     return
  }
  
  destroy .warning2
  update idletasks
  
  #  read in output data, one line at a time, parsing with lindex

  set f [open $tmp_dir/K[pid]canonicalPic.out r]
   
  set lineOne [gets $f]
  if {$lineOne == "non-hyperbolic"} {
    warning "Knot is apparently non-hyperbolic"
    return
  }
  set numCrossings [lindex $lineOne 0]
  set knotID [lindex $lineOne 1]
  set scale [lindex [gets $f] 0]
  set pictureWidth [expr 800*$scale]
  set pictureHeight [expr 600*$scale]
  
  # set up canvas

  toplevel .canonicalPicture
  wm geometry .canonicalPicture +20+20
  wm title .canonicalPicture [concat $numCrossings "." $knotID]
  wm withdraw .canonicalPicture
  
  ScrolledCanvas .canonicalPicture.fr1 800 600\
     [list 0  [expr $pictureHeight/2-300]  $pictureWidth  [expr $pictureHeight/2+300]]
  #canvas .canonicalPicture.c -width [expr 800*$scale] -height [expr 600*$scale] -bg white
  frame .canonicalPicture.fr2 -bg #00007d7008f6
  button .canonicalPicture.fr2.dismiss -bg #00d090 -activebackground #aaffff\
    -font "-adobe-new century schoolbook-medium-r-*-*-17-*-*-*-*-*-*-*"\
    -highlightthickness 1.5 -highlightbackground yellow\
    -text "Dismiss  (Ctrl-q)" -command [list canonicalPictureDelete]
  button .canonicalPicture.fr2.savePs -bg #00d090 -activebackground #aaffff\
    -font "-adobe-new century schoolbook-medium-r-*-*-17-*-*-*-*-*-*-*"\
    -highlightthickness 1.5 -highlightbackground yellow\
    -text "Save PostScript" -command [list canonicalSavePs]
  menubutton .canonicalPicture.fr2.zoom -bg #ffa0d0 -text "Zoom"\
    -font "-adobe-new century schoolbook-medium-r-*-*-17-*-*-*-*-*-*-*" -relief raised\
    -highlightthickness 2.5 -highlightbackground blue\
    -menu .canonicalPicture.fr2.zoom.menu -activebackground #00e0ff
  menu .canonicalPicture.fr2.zoom.menu -tearoff false
  
  foreach item {0.5 0.7 1.0 1.3 1.5} {
     .canonicalPicture.fr2.zoom.menu add command -label $item -background #d0d0ff\
        -activebackground #00e0ff -command "czoom $item"
  }
  
  bind .canonicalPicture <Control-q> {
     set windowName %W
     set windowNumber [string range $windowName 16 end]
     destroy $windowName
     if {[winfo exists .horozoom$windowNumber] == 1} {destroy .horozoom$windowNumber}
  }
  
  warning2 "drawing ..." 410 80
  raise .warning2
  update idletasks
 
  pack .canonicalPicture.fr1.c -side top
  pack .canonicalPicture.fr2 -side top -fill x -expand false
  pack .canonicalPicture.fr2.savePs .canonicalPicture.fr2.dismiss\
     -side left -pady 10 -padx 50
  pack .canonicalPicture.fr2.zoom -side right -pady 10 -padx 50
  
  # draw fundamental region

  set vertex [gets $f]  
  set x0 [lindex $vertex 0]
  set y0 [lindex $vertex 1]
  
  set vertex [gets $f]
  set x1 [lindex $vertex 0]
  set y1 [lindex $vertex 1]
  
  set vertex [gets $f]
  set x2 [lindex $vertex 0]
  set y2 [lindex $vertex 1]

  set vertex [gets $f]
  set x3 [lindex $vertex 0]
  set y3 [lindex $vertex 1]

  .canonicalPicture.fr1.c create line $x0 $y0 $x1 $y1 -width 4 -fill green
  .canonicalPicture.fr1.c create line $x1 $y1 $x2 $y2 -width 4 -fill green
  .canonicalPicture.fr1.c create line $x2 $y2 $x3 $y3 -width 4 -fill green
  .canonicalPicture.fr1.c create line $x3 $y3 $x0 $y0 -width 4 -fill green
  
  # draw circles
    
  set tst 1
 
  while {$tst == 1 } {
    set canonicalData [gets $f]
    if {$canonicalData == ""} {
       set tst 0
       continue
    }  
    set key [lindex $canonicalData 0]
    if {$key == "h"} {
      set u1 [lindex $canonicalData 1]
      set v1 [lindex $canonicalData 2]
      set u2 [lindex $canonicalData 3]
      set v2 [lindex $canonicalData 4]
      .canonicalPicture.fr1.c create oval $u1 $v1 $u2 $v2
      
    } elseif {$key == "s"} {
      set u1 [lindex $canonicalData 1]
      set v1 [lindex $canonicalData 2]
      set u2 [lindex $canonicalData 3]
      set v2 [lindex $canonicalData 4]
      .canonicalPicture.fr1.c create line $u1 $v1 $u2 $v2 -fill blue
      
    } elseif {$key == "b"} {
      set u1 [lindex $canonicalData 1]
      set v1 [lindex $canonicalData 2]
      set u2 [lindex $canonicalData 3]
      set v2 [lindex $canonicalData 4]
      .canonicalPicture.fr1.c create oval $u1 $v1 $u2 $v2 -outline white -fill white
      
    } elseif {$key == "l1"} {
      set u1 [lindex $canonicalData 1]
      set v1 [lindex $canonicalData 2]
      set label [lindex $canonicalData 3]
      .canonicalPicture.fr1.c create text $u1 $v1 -text $label -fill black\
         -font -adobe-helvetica-bold-r-*-*-17-*-*-*-*-*-*-*
         
    } elseif {$key == "l2"} {
      set u1 [lindex $canonicalData 1]
      set v1 [lindex $canonicalData 2]
      set label [lindex $canonicalData 3]
      .canonicalPicture.fr1.c create text $u1 $v1 -text $label -fill #a000a0\
         -font -adobe-helvetica-bold-r-*-*-17-*-*-*-*-*-*-*
         
    } elseif {$key == "q"} {
      set u1 [lindex $canonicalData 1]
      set v1 [lindex $canonicalData 2]
      set u2 [expr $u1-4]
      set v2 [expr $v1-4]
      set u3 [expr $u1+4]
      set v3 [expr $v1+4]
      .canonicalPicture.fr1.c create rectangle $u2 $v2 $u3 $v3 -fill yellow
    }
  }
  
  close $f
  
  destroy .warning2
  wm deiconify .canonicalPicture
  update idletasks

}

proc canonicalSavePs {} {
   global tmp_dir home_dir
   
   set file [fileselect "Save As ..." "$home_dir/ "]
   if {[string trim $file] != ""} {exec cp $tmp_dir/K[pid]canonical.ps $file}
}
  
proc canonicalPictureDelete {} {
   destroy .canonicalPicture
}

proc ScrolledCanvas {fr width height region} {
   frame $fr
   canvas $fr.c -width $width -height $height -bg white\
      -scrollregion $region\
      -xscrollcommand [list $fr.xscroll set]
   scrollbar $fr.xscroll -orient horizontal -command [list $fr.c xview]\
      -bg blue -activebackground #aa55ff -troughcolor #eebbee
   pack $fr.xscroll -side bottom -fill x
   pack $fr.c -side left -fill both -expand true
   pack $fr -side top -fill both -expand true
   return $fr.c
}

proc czoom {ratio} {
   global tmp_dir
   
   .canonicalPicture.fr1.c addtag del all
   .canonicalPicture.fr1.c delete tag del
   
   set f [open $tmp_dir/K[pid]canonicalPic.out r]
   
   set lineOne [gets $f]
   set numCrossings [lindex $lineOne 0]
   set knotID [lindex $lineOne 1]
   set scale [expr [lindex [gets $f] 0]*$ratio]
   set pictureWidth [expr 800*$scale]
   set pictureHeight [expr 600*$scale]

   .canonicalPicture.fr1.c configure -scrollregion\
      [list 0  [expr $pictureHeight/2-300]  $pictureWidth  [expr $pictureHeight/2+300]]
   
   warning2 "drawing ..." 410 80
   raise .warning2
   update idletasks
   
   set circleColor "#000088"
 
   # draw fundamental region

   set vertex [gets $f]
   set x0 [expr [lindex $vertex 0]*$ratio]
   set y0 [expr [lindex $vertex 1]*$ratio]
 
   set vertex [gets $f]
   set x1 [expr [lindex $vertex 0]*$ratio]
   set y1 [expr [lindex $vertex 1]*$ratio]
 
   set vertex [gets $f]
   set x2 [expr [lindex $vertex 0]*$ratio]
   set y2 [expr [lindex $vertex 1]*$ratio]

   set vertex [gets $f]
   set x3 [expr [lindex $vertex 0]*$ratio]
   set y3 [expr [lindex $vertex 1]*$ratio]

   .canonicalPicture.fr1.c create line $x0 $y0 $x1 $y1 -width 4 -fill green
   .canonicalPicture.fr1.c create line $x1 $y1 $x2 $y2 -width 4 -fill green
   .canonicalPicture.fr1.c create line $x2 $y2 $x3 $y3 -width 4 -fill green
   .canonicalPicture.fr1.c create line $x3 $y3 $x0 $y0 -width 4 -fill green
 
   # draw circles
 
   set tst 1
 
   while {$tst == 1 } {
     set canonicalData [gets $f]
     if {$canonicalData == ""} {
        set tst 0
        continue
     }
     set key [lindex $canonicalData 0]
     if {$key == "h"} {
       set u1 [expr [lindex $canonicalData 1]*$ratio]
       set v1 [expr [lindex $canonicalData 2]*$ratio]
       set u2 [expr [lindex $canonicalData 3]*$ratio]
       set v2 [expr [lindex $canonicalData 4]*$ratio]
       .canonicalPicture.fr1.c create oval $u1 $v1 $u2 $v2
       
     } elseif {$key == "s"} {
       set u1 [expr [lindex $canonicalData 1]*$ratio]
       set v1 [expr [lindex $canonicalData 2]*$ratio]
       set u2 [expr [lindex $canonicalData 3]*$ratio]
       set v2 [expr [lindex $canonicalData 4]*$ratio]
       .canonicalPicture.fr1.c create line $u1 $v1 $u2 $v2 -fill blue
       
     } elseif {$key == "b"} {
       set u1 [expr [lindex $canonicalData 1]*$ratio]
       set v1 [expr [lindex $canonicalData 2]*$ratio]
       set u2 [expr [lindex $canonicalData 3]*$ratio]
       set v2 [expr [lindex $canonicalData 4]*$ratio]
       .canonicalPicture.fr1.c create oval $u1 $v1 $u2 $v2 -outline white -fill white
     
     } elseif {$key == "l1"} {
       set u1 [expr [lindex $canonicalData 1]*$ratio]
       set v1 [expr [lindex $canonicalData 2]*$ratio]
       set label [lindex $canonicalData 3]
       .canonicalPicture.fr1.c create text $u1 $v1 -text $label -fill black\
          -font -adobe-helvetica-bold-r-*-*-17-*-*-*-*-*-*-*
          
     } elseif {$key == "l2"} {
       set u1 [expr [lindex $canonicalData 1]*$ratio]
       set v1 [expr [lindex $canonicalData 2]*$ratio]
       set label [lindex $canonicalData 3]
       .canonicalPicture.fr1.c create text $u1 $v1 -text $label -fill #880088\
          -font -adobe-helvetica-bold-r-*-*-17-*-*-*-*-*-*-*
          
     } elseif {$key == "q"} {
       set u1 [expr [lindex $canonicalData 1]*$ratio]
       set v1 [expr [lindex $canonicalData 2]*$ratio]
       set u2 [expr $u1-4]
       set v2 [expr $v1-4]
       set u3 [expr $u1+4]
       set v3 [expr $v1+4]
       .canonicalPicture.fr1.c create rectangle $u2 $v2 $u3 $v3 -fill yellow
 
     }
  }
  
  close $f
  
  destroy .warning2
  wm deiconify .canonicalPicture
  update idletasks
   
}

#############################################################################
#                           Save Triangulation                              #
#############################################################################

proc saveKnotTriangulation {} {

  global ks_dir tmp_dir home_dir
  
  #  This procedure allows the complement of a single knot from the Input
  #  Window to be saved in SnapPea manifold format.
  
  #  read single knot from selection, or from line 1 of input text widget if
  #  nothing is selected

  if {[string length [.input.fr1.text tag ranges sel]] == 0} {
    set knotData [.input.fr1.text get 1.0 1.end]
  } else {
    set selFirstIndex [.input.fr1.text index sel.first]
    scan $selFirstIndex "%d.%d" selFirstLine selFirstChar
    set selLastIndex [.input.fr1.text index sel.last]
    scan $selLastIndex "%d.%d" selLastLine selLastChar
    if {!($selFirstLine == $selLastLine ||\
         ([incr selFirstLine] == $selLastLine && $selLastChar == 0))} {
      warning "Please select single knot for conversion to manifold format"
      return
    }
    
    set knotData [.input.fr1.text get sel.first sel.last]
  }
  
  if {[checkIntegrity $knotData] == 1} {return}

  set f [open $tmp_dir/K[pid]knotfile w]
  puts $f $knotData
  close $f
  
  set str1 [fileselect "Save As ..." "$home_dir/ "]
 
  if {[string trim $str1] != ""} {
    set str2 [string trimleft $str1 "/"]
    set triangulationFile "/"
    append triangulationFile $str2
  
    if {[catch {exec $ks_dir/bin/knotscape_hyp 0 $tmp_dir/K[pid]knotfile $triangulationFile}] == 1} {
      warning "An error has occurred in the conversion to manifold format"
      return
    }
  }
}


#############################################################################
#                            Horoball Diagram                               #
#############################################################################

proc horoballDiagram {cut_off} {

  global ks_dir tmp_dir xcoord ycoord
  
  #  This procedure allows multiple horoball diagrams and zoom windows.  
  #  The horoball top-level windows are named .horoballPicture1, .horoballPicture2
  #  etc.  We begin by finding a free index, stored in the variable nhw.
  
  set nhw 0
  while {[winfo exists .horoballPicture$nhw] == 1} {incr nhw}  

  #  read single knot from selection, or from line 1 of input text widget if
  #  nothing is selected

  set f [open $tmp_dir/K[pid]horopic$nhw.in w]
  
  if {[string length [.input.fr1.text tag ranges sel]] == 0} {
    set knotData [.input.fr1.text get 1.0 1.end]
  } else {
    set selFirstIndex [.input.fr1.text index sel.first]
    scan $selFirstIndex "%d.%d" selFirstLine selFirstChar
    set selLastIndex [.input.fr1.text index sel.last]
    scan $selLastIndex "%d.%d" selLastLine selLastChar
    if {!($selFirstLine == $selLastLine ||\
         ([incr selFirstLine] == $selLastLine && $selLastChar == 0))} {
      warning "Please select single knot for horoball diagram"
      return
    }
    
    set knotData [.input.fr1.text get sel.first sel.last]
  }
   
  set hpicInput "$knotData  $cut_off"
  
  puts $f $hpicInput

  close $f
  
  if {[checkIntegrity $knotData] == 1} {return}
  
  update idletasks
  
  warning2 "Computing ..." 400 80
  update idletasks
     
  if {[catch {exec $ks_dir/bin/knotscape_hyp 4 $tmp_dir/K[pid]horopic$nhw.in\
     $tmp_dir/K[pid]horopic$nhw.out $tmp_dir/K[pid]horoballs$nhw.ps}] == 1} {
     destroy .warning2
     warning "An error has occurred in the computation of the horoballs"
     return
  }
  
  destroy .warning2
  update idletasks
  
  #  read in output data, one line at a time, parsing with lindex

  set f [open $tmp_dir/K[pid]horopic$nhw.out r]
   
  set lineOne [gets $f]
  if {$lineOne == "non-hyperbolic"} {
    warning "Knot is apparently non-hyperbolic"
    return
  }
  set numCrossings [lindex $lineOne 0]
  set knotID [lindex $lineOne 1]

  # set up canvas

  toplevel .horoballPicture$nhw
  wm geometry .horoballPicture$nhw +[expr 180+40*$nhw]+[expr 40*$nhw]
  wm title .horoballPicture$nhw [concat $numCrossings "." $knotID\
     ";     cut-off diameter = " $cut_off]
  wm withdraw .horoballPicture$nhw
  
  canvas .horoballPicture$nhw.c -width 800 -height 600 -bg white
  frame .horoballPicture$nhw.fr -bg #00007d7008f6
  button .horoballPicture$nhw.fr.dismiss -bg #00d090 -activebackground #aaffff\
    -font "-adobe-new century schoolbook-medium-r-*-*-17-*-*-*-*-*-*-*"\
    -highlightthickness 1.5 -highlightbackground yellow\
    -text "Dismiss  (Ctrl-q)" -command [list horoballPictureDelete $nhw]
  button .horoballPicture$nhw.fr.savePs -bg #00d090 -activebackground #aaffff\
    -font "-adobe-new century schoolbook-medium-r-*-*-17-*-*-*-*-*-*-*"\
    -highlightthickness 1.5 -highlightbackground yellow\
    -text "Save PostScript" -command [list horoballSavePs $nhw]
  message .horoballPicture$nhw.fr.msg -bg #00d090 -text "Click on drawing to zoom"\
    -font "-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-*-*"\
    -width 200 -relief raised
  
  bind .horoballPicture$nhw <Control-q> {
     set windowName %W
     set windowNumber [string range $windowName 16 end]
     destroy $windowName
     if {[winfo exists .horozoom$windowNumber] == 1} {destroy .horozoom$windowNumber}
  }
  
  warning2 "drawing ..." 410 80
  raise .warning2
  update idletasks
 
  pack .horoballPicture$nhw.c -side top
  pack .horoballPicture$nhw.fr -side top -fill both -expand true
  pack .horoballPicture$nhw.fr.savePs .horoballPicture$nhw.fr.dismiss\
     -side left -pady 10 -padx 50
  pack .horoballPicture$nhw.fr.msg -side right -pady 10 -padx 50
  
  
  set circleColor "#000088"
  
  # draw fundamental region

  set vertex [gets $f]  
  set x0 [lindex $vertex 0]
  set y0 [expr 600-[lindex $vertex 1]]
  
  set vertex [gets $f]
  set x1 [lindex $vertex 0]
  set y1 [expr 600-[lindex $vertex 1]]
  
  set vertex [gets $f]
  set x2 [lindex $vertex 0]
  set y2 [expr 600-[lindex $vertex 1]]

  set vertex [gets $f]
  set x3 [lindex $vertex 0]
  set y3 [expr 600-[lindex $vertex 1]]

  .horoballPicture$nhw.c create line $x0 $y0 $x1 $y1
  .horoballPicture$nhw.c create line $x1 $y1 $x2 $y2
  .horoballPicture$nhw.c create line $x2 $y2 $x3 $y3
  .horoballPicture$nhw.c create line $x3 $y3 $x0 $y0
  
  
  # draw circles
    
  set tst 1
 
  while {$tst == 1 } {
    set horoballData [gets $f]
    if {$horoballData == ""} {
       set tst 0
       continue
    }  
    set u1 [lindex $horoballData 0]
    set v1 [expr 600-[lindex $horoballData 1]]
    set u2 [lindex $horoballData 2]
    set v2 [expr 600-[lindex $horoballData 3]]
    
    .horoballPicture$nhw.c create oval $u1 $v1 $u2 $v2
         
  }
  
  
  close $f
  
  destroy .warning2
  wm deiconify .horoballPicture$nhw
  update idletasks

  
  bind .horoballPicture$nhw.c <ButtonPress> {

    global hzn

    set windowName %W
    set nameLength [string length $windowName]
    incr nameLength -3
    set hzn [string range $windowName 16 $nameLength]
    
    if {[winfo exists .horozoom$hzn] == 1} {return}
    
    set f [open $tmp_dir/K[pid]horopic$hzn.in r]
    set knotData [gets $f]
    close $f
    
    set numCrossings [lindex $knotData 0]
    set knotID [lindex $knotData 1]
    set pos [expr $numCrossings+2]
    set cut_off [lindex $knotData $pos]
    
      
    toplevel .horozoom$hzn
    wm geometry .horozoom$hzn 800x850+[expr 50+30*$hzn]+[expr 50+30*$hzn]
    
    wm title .horozoom$hzn [concat $numCrossings "." $knotID\
     ";     cut-off diameter = " $cut_off]

    bind .horozoom$hzn <Control-q> {
      set windowName %W
      set nameLength [string length $windowName]
      incr nameLength -3
      set windowNumber [string range $windowName 16 $nameLength]
      destroy .horozoom$windowNumber
    }
   
    canvas .horozoom$hzn.d -bg white -height 800 -width 800
    frame .horozoom$hzn.fr -bg #8888ff  
    button .horozoom$hzn.fr.dismiss -bg #ff88ee -activebackground #aaffff\
     -highlightthickness 2 -highlightbackground #00ffff\
     -text "Dismiss  (Ctrl-q)" -command [list horozoomPictureDelete $hzn]
    pack .horozoom$hzn.d -side top
    pack .horozoom$hzn.fr -side top -fill both -expand true
    pack .horozoom$hzn.fr.dismiss -pady 10
 
    set f [open $tmp_dir/K[pid]mousecoords w]
    puts $f "%x        [expr 600-%y]"
    close $f
    
    if {[catch {exec $ks_dir/bin/horozoom $tmp_dir/K[pid]mousecoords\
       $tmp_dir/K[pid]horopic$hzn.out $tmp_dir/K[pid]horozoom.out}] == 1} {
       destroy .horozoom$hzn
       warning "An error has occurred while zooming"
       return
    }
    
    set f [open $tmp_dir/K[pid]horozoom.out r]

    set vertex [gets $f]  
    set x0 [lindex $vertex 0]
    set y0 [expr 800-[lindex $vertex 1]]
    
    set vertex [gets $f]
    set x1 [lindex $vertex 0]
    set y1 [expr 800-[lindex $vertex 1]]
    
    set vertex [gets $f]
    set x2 [lindex $vertex 0]
    set y2 [expr 800-[lindex $vertex 1]]
  
    set vertex [gets $f]
    set x3 [lindex $vertex 0]
    set y3 [expr 800-[lindex $vertex 1]]
  
    .horozoom$hzn.d create line $x0 $y0 $x1 $y1
    .horozoom$hzn.d create line $x1 $y1 $x2 $y2
    .horozoom$hzn.d create line $x2 $y2 $x3 $y3
    .horozoom$hzn.d create line $x3 $y3 $x0 $y0
         
    set tst 1
     
    while {$tst == 1} {
      set horoballData [gets $f]

      if {$horoballData == ""} {
         set tst 0
         continue
      }

      set u1 [lindex $horoballData 0]
      set v1 [expr 800-[lindex $horoballData 1]]
      set u2 [lindex $horoballData 2]
      set v2 [expr 800-[lindex $horoballData 3]]
       
      .horozoom$hzn.d create oval $u1 $v1 $u2 $v2    
    }
    close $f
  
    
    update idletasks
  }
}

proc horoballSavePs {nhw} {
   global tmp_dir home_dir
   
   set file [fileselect "Save As ..." "$home_dir/ "]
   if {[string trim $file] != ""} {exec cp $tmp_dir/K[pid]horoballs$nhw.ps $file}
}
  
proc horoballPictureDelete {nhw} {
   destroy .horoballPicture$nhw
   if {[winfo exists .horozoom$nhw] == 1} {
      destroy .horozoom$nhw
   }
}

proc horozoomPictureDelete {hzn} {
   destroy .horozoom$hzn
}


#############################################################################
#                  procedure for making PostScript files                    #
#############################################################################

proc psKnot {} {

  global ks_dir tmp_dir
  
  
  if {[string length [.input.fr1.text tag ranges sel]] == 0} {
      set selectedKnots [.input.fr1.text get 1.0 1.end]
   } else {
      set selectedKnots [.input.fr1.text get sel.first sel.last]
   }
  
   set f0 [open $tmp_dir/K[pid]knotfile0 w]
   puts $f0 $selectedKnots
   close $f0
   
   set f0 [open $tmp_dir/K[pid]knotfile0 r]

   warning2 "Working ..." 400 80
   update idletasks
   
   set fileNumber 0

   while {[gets $f0 str] >= 0} {

      set str [string trim $str]
      if {[string length $str] == 0} continue

      set inputList [list $str]

# check integrity of data

      set str [string trim $str]

      if {[checkIntegrity $str] == 1} {
         if {[winfo exists .warning2]} {destroy .warning2}
         return
      }
   
      set f1 [open $tmp_dir/K[pid]knotfile1 w]
      puts $f1 $str
      close $f1
 
      if {[catch {exec $ks_dir/bin/draw $tmp_dir/K[pid]knotfile1 $tmp_dir/K[pid]triang.out\
         $tmp_dir/K[pid]ken.out $tmp_dir/K[pid]vcoords.out $tmp_dir/K[pid]segcoords.out}] == 1} {
         destroy .warning2
         warning "An error occurred in the knot drawing procedure"
         return
      }
      
      incr fileNumber
 
      if {[catch {exec $ks_dir/bin/draw2ps $tmp_dir/K[pid]knotfile1 $tmp_dir/K[pid]segcoords.out\
         $tmp_dir/K[pid]knotps_$fileNumber.ps}] == 1} {
         destroy .warning2
         warning "An error occurred in the PostScript procedure"
         return
      }
 
   }
   
   destroy .warning2
   update idletasks
}



#############################################################################
#                      procedure for drawing knot                           #
#############################################################################

proc drawKnot {} {

  global ks_dir tmp_dir
  
  set ndw 0
  while {[winfo exists .drawing$ndw] == 1} {incr ndw}
    
# read knot from selection, or from first line of input text widget if
# nothing is selected

  if {[string length [.input.fr1.text tag ranges sel]] == 0} {
    set knotData [.input.fr1.text get 1.0 1.end]
  } else {
    set selFirstIndex [.input.fr1.text index sel.first]
    scan $selFirstIndex "%d.%d" selFirstLine selFirstChar
    set selLastIndex [.input.fr1.text index sel.last]
    scan $selLastIndex "%d.%d" selLastLine selLastChar
    if {!($selFirstLine == $selLastLine ||\
         ([incr selFirstLine] == $selLastLine && $selLastChar == 0))} {
      warning "Please select single knot for drawing"
      return
    }
  
    set knotData [.input.fr1.text get sel.first sel.last]
  }
  
  set f [open $tmp_dir/K[pid]knotfile$ndw w]
  puts $f $knotData
  close $f
  
  if {[checkIntegrity $knotData] == 1} {return}
  
  warning2 "Drawing ..." 400 80
  update idletasks
    
  set numCrossings [lindex $knotData 0]
  set knotID [lindex $knotData 1]
  set knotList [lrange $knotData 2 end]

  set numStrands [expr 2*$numCrossings]

  #set up Dowker table

  for {set i 1} {$i <= [expr $numCrossings]} {incr i} {
    set j [lindex $knotList [expr $i-1]]
    set over([expr 2*$i-1]) [expr $j / abs($j)]
    set dowker([expr 2*$i-1]) [expr abs($j)]
  }

  for {set i 1} {$i <= [expr $numStrands - 1]} {incr i 2} {
    set dowker($dowker($i)) $i
    set over($dowker($i)) [expr -$over($i)]
  }
  
  set f [open $tmp_dir/K[pid]knotfile w]
  puts $f $numCrossings
  puts $f $knotID
  puts $f $knotList
  close $f
  
  if {[catch {exec $ks_dir/bin/draw $tmp_dir/K[pid]knotfile$ndw $tmp_dir/K[pid]triang.out\
     $tmp_dir/K[pid]ken.out $tmp_dir/K[pid]vcoords$ndw.out $tmp_dir/K[pid]segcoords$ndw.out}] == 1} {
     destroy .warning2
     warning "An error occurred in the knot drawing procedure"
     return
  }

  set numStrands [expr 2*$numCrossings]

  #set up Dowker table

  for {set i 1} {$i <= [expr $numCrossings]} {incr i} {
    set j [lindex $knotList [expr $i-1]]
    set over([expr 2*$i-1]) [expr $j / abs($j)]
    set dowker([expr 2*$i-1]) [expr abs($j)]
  }

  for {set i 1} {$i <= [expr $numStrands - 1]} {incr i 2} {
    set dowker($dowker($i)) $i
    set over($dowker($i)) [expr -$over($i)]
  }

  # read in list of coordinates of crossings (before smoothing)

  set f [open $tmp_dir/K[pid]vcoords$ndw.out r]
  set crossList [read $f]
  close $f

  # read in coordinates for drawing separate crossing strands

  set f [open $tmp_dir/K[pid]segcoords$ndw.out r]
  for {set i 1} {$i <= $numStrands} {incr i} {
    set shortStrandList($i) [gets $f]
  }

  for {set i 1} {$i <= $numStrands} {incr i} {
    set longStrandList($i) [gets $f]
  }

  close $f


  # set up canvas

  toplevel .drawing$ndw
  wm geometry .drawing$ndw +[expr 130+40*$ndw]+[expr 40*$ndw]
  wm title .drawing$ndw [concat $numCrossings "." $knotID]
 
  canvas .drawing$ndw.c -width 640 -height 640 -bg white
  frame .drawing$ndw.fr -bg blue
  button .drawing$ndw.fr.savePs -bg #00d090 -activebackground #aaffff\
    -highlightthickness 2 -highlightbackground #ff0000\
    -text "Save PostScript" -command [list knotPictureSavePs $ndw]
  button .drawing$ndw.fr.dismiss -bg #ff88ee -activebackground #aaffff\
    -highlightthickness 2 -highlightbackground #ff0000\
    -text "Dismiss  (Ctrl-q)" -command [list knotPictureDelete $ndw]
  message .drawing$ndw.fr.msg -bg #00d090 -text "Click on drawing to zoom"\
    -font -adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-*-*\
    -width 200 -relief raised
    
  
  bind .drawing$ndw <Control-q> {
     set windowName %W
     set windowNumber [string range $windowName 8 end]
     destroy $windowName
     if {[winfo exists .zoom$windowNumber] == 1} {destroy .zoom$windowNumber}
  }  
  

  pack .drawing$ndw.c -side top
  pack .drawing$ndw.fr -side top -fill both -expand true
  pack .drawing$ndw.fr.savePs -side left -pady 10 -padx 20
  pack .drawing$ndw.fr.dismiss -side left -pady 10 -padx 20
  pack .drawing$ndw.fr.msg -side right -pady 10 -padx 20
  
  set knotColor "#00c377"
  #set knotColor black

  # set up parts of canvas item creation commands
  

  if {$numCrossings <= 6} {
    set knotWidth 8
    set gapWidth 20
  } elseif {$numCrossings <= 15} {
    set knotWidth 5
    set gapWidth 15
  } else {
    set knotWidth 2
    set gapWidth 8
  }

  set createPoly ".drawing$ndw.c create polygon"
  set createLine ".drawing$ndw.c create line"
  set configPoly "-width $knotWidth -outline $knotColor -fill white -smooth 1"
  set configLine "-width $knotWidth -fill $knotColor -smooth 1"
  set configWideLine " -width $gapWidth -fill white -smooth 1"

  # draw polygon

  set cross [join $crossList]

  eval [concat $createPoly $cross $configPoly]

  # draw strands

  for {set i 1} {$i < $numStrands} {incr i 2} {
    if {$over($i) == 1} {

      set shortStrand [join $shortStrandList($i)]
      set longStrand [join $longStrandList($i)]
    } else {
  
      set j $dowker($i)
      set shortStrand [join $shortStrandList($j)]
      set longStrand [join $longStrandList($j)]
    }
  
    eval [concat $createLine $shortStrand $configWideLine]
    eval [concat $createLine $longStrand $configLine]
  }


  update idletasks

  .drawing$ndw.c postscript -colormode color -file $tmp_dir/K[pid]knot$ndw.ps
  update idletasks
  set f [open $tmp_dir/K[pid]knot_data w]
  puts $f $knotID
  puts $f $numCrossings
  puts $f $knotList
  close $f
  
  destroy .warning2
  update idletasks
    
  bind .drawing$ndw.c <ButtonPress> {
    
    global dzn
    
    set windowName %W
    set nameLength [string length $windowName]
    incr nameLength -3
    set dzn [string range $windowName 8 $nameLength]
    
    if {[winfo exists .zoom$dzn] == 1} {return}
      
    toplevel .zoom$dzn
    wm geometry .zoom$dzn 600x650+[expr 250+30*$dzn]+[expr 100+30*$dzn]
    
    bind .zoom$dzn <Control-q> {
      set windowName %W
      set nameLength [string length $windowName]
      incr nameLength -3
      set windowNumber [string range $windowName 8 $nameLength]
      destroy .zoom$windowNumber
    }    

    canvas .zoom$dzn.d -bg white -height 600 -width 600
    frame .zoom$dzn.fr -bg #8888ff  
    button .zoom$dzn.fr.dismiss -bg #ff88ee -activebackground #aaffff\
     -text "Dismiss  (Ctrl-q)" -command {destroy .zoom$dzn}
      
    pack .zoom$dzn.d -side top
    pack .zoom$dzn.fr -side top -fill both -expand true
    pack .zoom$dzn.fr.dismiss -pady 10
    
    set f [open $tmp_dir/K[pid]knotfile$dzn r]
    set knotData [gets $f]
    close $f
    
    set numCrossings [lindex $knotData 0]
    set knotID [lindex $knotData 1]
    set knotList [lrange $knotData 2 end]
    
    wm title .zoom$dzn [concat $numCrossings "." $knotID]
    
    set numStrands [expr 2*$numCrossings]
    set numNodes [expr 4*$numCrossings]
        
    # read in list of coordinates of crossings (before smoothing)

    set f [open $tmp_dir/K[pid]vcoords$dzn.out r]
    set crossList [read $f]
    close $f

    # read in coordinates for drawing separate crossing strands

    set f [open $tmp_dir/K[pid]segcoords$dzn.out r]
    for {set i 1} {$i <= $numStrands} {incr i} {
      set shortStrandList($i) [gets $f]
    }

    for {set i 1} {$i <= $numStrands} {incr i} {
      set longStrandList($i) [gets $f]
    }

    close $f
    
    set zoomCrossList {}
    set zoomShortStrand {}
    set zoomLongStrand {} 
       
    for {set i 1} {$i <= $numNodes} {incr i} {
      set oldx [lindex $crossList [expr 2*($i-1)]]
      set oldy [lindex $crossList [expr 2*($i-1) + 1]]
      set newx [expr 5*($oldx - %x) + 300]
      set newy [expr 5*($oldy - %y) + 300]
      lappend zoomCrossList $newx $newy
    }
    
    for {set i 1} {$i <= $numStrands} {incr i} {
      set zoomShortStrandList($i) {}
      
      for {set j 1} {$j <= 3} {incr j} {         
        set oldx [lindex $shortStrandList($i) [expr 2*($j-1)]]
        set oldy [lindex $shortStrandList($i) [expr 2*($j-1) + 1]]
        set newx [expr 5*($oldx - %x) + 300]
        set newy [expr 5*($oldy - %y) + 300]
        lappend zoomShortStrandList($i) $newx $newy
      }
    }
    
    for {set i 1} {$i <= $numStrands} {incr i} {
      set zoomLongStrandList($i) {}
      
      for {set j 1} {$j <= 5} {incr j} {         
        set oldx [lindex $longStrandList($i) [expr 2*($j-1)]]
        set oldy [lindex $longStrandList($i) [expr 2*($j-1) + 1]]
        set newx [expr 5*($oldx - %x) + 300]
        set newy [expr 5*($oldy - %y) + 300]
        lappend zoomLongStrandList($i) $newx $newy
      }
    }
        
    #set up Dowker table

    for {set i 1} {$i <= [expr $numCrossings]} {incr i} {
      set j [lindex $knotList [expr $i-1]]
      set over([expr 2*$i-1]) [expr $j / abs($j)]
      set dowker([expr 2*$i-1]) [expr abs($j)]
    }

    for {set i 1} {$i <= [expr $numStrands - 1]} {incr i 2} {
      set dowker($dowker($i)) $i
      set over($dowker($i)) [expr -$over($i)]
    }

    set knotColor "#aa00ff"    
    set createPoly ".zoom$dzn.d create polygon"
    set createLine ".zoom$dzn.d create line"
    set configPoly "-width 3 -outline $knotColor -fill white -smooth 1"
    set configLine "-width 3 -fill $knotColor -smooth 1"
    set configWideLine " -width 12 -fill white -smooth 1" 
    
    # draw polygon

    set zoomCross [join $zoomCrossList]

    eval [concat $createPoly $zoomCross $configPoly]


    # draw strands

    for {set i 1} {$i < $numStrands} {incr i 2} {
      if {$over($i) == 1} {

        set zoomShortStrand [join $zoomShortStrandList($i)]
        set zoomLongStrand [join $zoomLongStrandList($i)]
      } else {

        set j $dowker($i)
        set zoomShortStrand [join $zoomShortStrandList($j)]
        set zoomLongStrand [join $zoomLongStrandList($j)]
      }

      eval [concat $createLine $zoomShortStrand $configWideLine]
      eval [concat $createLine $zoomLongStrand $configLine]
    }

  }

}

proc knotPictureSavePs {ndw} {
   global tmp_dir home_dir
   
   set file [fileselect "Save As ..." "$home_dir/ "]
   if {[string trim $file] != ""} {exec cp $tmp_dir/K[pid]knot$ndw.ps $file}
}

proc knotPictureDelete {ndw} {
   destroy .drawing$ndw
   if {[winfo exists .zoom$ndw] == 1} {
      destroy .zoom$ndw
   }
}

proc zoomDelete {dzn} {
   destroy .zoom$dzn
}




#############################################################################
#            procedure for setting up help hypertext database               #
#############################################################################

proc showHelp {page} {

#text has two kinds of tags: "s"=section, "e"=example
#hypertext effect is obtained by bindings on tags

#initialize database for tags

   global ks_dir tmp_dir
   global sStringList eStringList sFileList eFileList currentTag
   global sTagNumber eTagNumber
   
#set up help window

   toplevel .help
   wm geometry .help 650x750+200+0
  
   frame .help.fr1
   frame .help.options -bg #c48af1
   
   text .help.fr1.text -yscrollcommand ".help.fr1.scroll set"\
      -height 30 -width 60 -bg #eddeff\
      -font -adobe-helvetica-medium-r-*-*-17-*-*-*-*-*-*-*
   scrollbar .help.fr1.scroll -command ".help.fr1.text yview" -bg blue\
      -activebackground #aa55ff -troughcolor #c48af1  
      
   button .help.options.close -text "Close Window"\
        -command "destroy .help; return"\
        -activebackground #ff70ff -bg #b7a7ff
               
   pack .help.fr1 -side top -fill both -expand true
   pack .help.options -side top -fill both -expand false
   pack .help.fr1.text -side left -fill both -expand true
   pack .help.fr1.scroll -side left -fill both -expand false
   pack .help.options.close -side left -padx 40\
        -pady 10
   
   .help.fr1.text config -spacing1 5
   
#set up database for tags
      
   set sStringList {\
      "Table of Contents"\
      "Tutorial"\
      "Notation for Knots"\
      "Entering a Knot in the Input Window"\
      "Making a Knot with LinkSmith"\
      "Locating a Knot in the Table"\
      "Generating a Picture of a Knot"\
      "Hyperbolic Invariants"\
      "Polynomials"\
      "Homomorphisms"\
      "Keyboard Shortcuts"\
      "Operational Limits"\
      "Editing in Text Windows"\
   }
   
   set sFileList {\
      toc\
      tutorial\
      notation\
      enter\
      linksmith\
      locate\
      draw\
      hyperbolic\
      polynomials\
      homomorphisms\
      shortcuts\
      limits\
      editing\
   }
   
   set eStringList {\
      "Dowker Code Example"\
      "Illustration of Polynomial Formats"\
      "Turk's Head Polytope"
   }
   
   set eFileList {\
      $ks_dir/tcl/fig8.tcl\
      $ks_dir/tcl/polyformat.tcl
      $ks_dir/tcl/turkPolytope.tcl
   }
   
#display Table of Contents in help text widget, and set up tags
   
   helpRead $page
      
}

#############################################################################
#      procedure for loading help files and setting up hypertext tags       #         
#############################################################################

proc helpRead {helpId} {
 
   global ks_dir tmp_dir
   
   set helpFile $ks_dir/help/help.$helpId
   set titleGif $ks_dir/gif/help.$helpId.gif
   
   set f [open $helpFile r]
   
   .help.fr1.text delete 1.0 end
   
#insert title picture

   set titlePic [image create photo -file $titleGif]

   .help.fr1.text window create 1.0\
       -create "label .help.fr1.text.title -image $titlePic\
       -width 446 -height 99 -bd 5"
   .help.fr1.text tag add titleTag 1.0 1.end
   .help.fr1.text tag configure titleTag -justify center
   .help.fr1.text insert end "\n\n"

   
   set sTagNumber 0
   set eTagNumber 0
   update idletasks

#read in help topic from file.  Tags are coded as follows:
#   (i) any line containing a tag begins with "&"
#   (ii) "s" tags begin with "@"
#   (iii) "e" tags begin with "#"
#   (iv) all tags end with "%"
   
   while {[gets $f currentLine] != -1} {
      if {[string index $currentLine 0] != "&"} {
         .help.fr1.text insert end $currentLine end "\n"
      } else {
         set str [string trimleft $currentLine &]
       
         for {set i 0} {$i < [string length $str]} {incr i} {
            set char [string index $str $i]
             
            if {$char == "@"} {
               set tagType "S"
               incr sTagNumber
               set tagStart [.help.fr1.text index insert]
            } elseif {$char == "#"} {
               set tagType "E"
               incr eTagNumber
               set tagStart [.help.fr1.text index insert]
            } elseif {$char == "%"} {
               set tagEnd [.help.fr1.text index insert]
               if {$tagType == "S"} {
                  .help.fr1.text tag add tagS$sTagNumber $tagStart $tagEnd
               } else {
                  .help.fr1.text tag add tagE$eTagNumber $tagStart $tagEnd
               }
            } else {
               .help.fr1.text insert insert $char
            } 
         }
         
         .help.fr1.text insert insert "\n"
      }
   }

   close $f
    
#configure tags and set up bindings
#sections are displayed in help text window using "helpRead"
     
   for {set item 1} {$item <= $sTagNumber} {incr item} {
      .help.fr1.text tag configure tagS$item\
         -foreground #0000c0 -spacing1 2\
         -font -adobe-times-medium-r-*-*-20-*-*-*-*-*-*-*
      .help.fr1.text tag bind tagS$item <Enter> {
         set ind [.help.fr1.text index @%x,%y]
         set currentTag [.help.fr1.text tag names $ind]
         .help.fr1.text tag configure $currentTag -foreground red
      }
      .help.fr1.text tag bind tagS$item <Leave> {
         .help.fr1.text tag configure $currentTag -foreground #0000c0
      }
      .help.fr1.text tag bind tagS$item <ButtonPress> {
         set str [.help.fr1.text get $currentTag.first $currentTag.last]
         set fileNumber [lsearch $sStringList $str]
         set helpId [lindex $sFileList $fileNumber]
         eval helpRead $helpId
      }
   }

#examples are run as tcl scripts
   
   for {set item 1} {$item <= $eTagNumber} {incr item} {
      .help.fr1.text tag configure tagE$item\
         -background #00f0b0 -relief raised -borderwidth 1 -spacing1 2\
         -font -adobe-times-medium-r-*-*-20-*-*-*-*-*-*-*
      .help.fr1.text tag bind tagE$item <Enter> {
         set ind [.help.fr1.text index @%x,%y]
         set currentTag [.help.fr1.text tag names $ind]
         .help.fr1.text tag configure $currentTag -background #55ffff
      }
      .help.fr1.text tag bind tagE$item <Leave> {
         .help.fr1.text tag configure $currentTag -background #00f0b0
      }
      .help.fr1.text tag bind tagE$item <ButtonPress> {
         set str [.help.fr1.text get $currentTag.first $currentTag.last]
         set fileNumber [lsearch $eStringList $str]
         set exampleFile [lindex $eFileList $fileNumber]
         if {[winfo exists .ex$fileNumber] != 1} {eval source $exampleFile}
      }
   }   
   
   update idletasks        
}

############################################################################
#                            output window                                 #       
############################################################################

proc showOutputWindow {init} {
  
   global ks_dir tmp_dir
   
   if {$init=="firstTime"} {
   
      global outputFilename
      set outputFilename "None"
      
      toplevel .output
      wm withdraw .output
      wm geometry .output 900x500+100+180
      
      bind .output <Enter> {focus %W}

      frame .output.fr1
      frame .output.fr2 -bg #f9c1ff

      # create output text window

      text .output.fr1.text -yscrollcommand ".output.fr1.scroll set"\
        -height 30 -width 95 
        
      scrollbar .output.fr1.scroll -command ".output.fr1.text yview" -bg blue\
        -activebackground #aa55ff -troughcolor #c48af1
         
      button .output.fr2.close -text "Close Window  (Ctrl-q)"\
        -command ".output.fr1.text delete 1.0 end; wm withdraw .output"\
        -activebackground #ff70ff -bg #b7a7ff
      button .output.fr2.saveAs -text "Save as ..  (Ctrl-s)" -command "saveAs .output"\
        -activebackground #ff70ff -bg #b7a7ff        
      
      bindtags .output [list %W all Text]
      bind .output <Control-q> {
        .output.fr1.text delete 1.0 end
        wm withdraw .output
	return -code break }
      bind .output <Control-s> {saveAs .output; return -code break}
      # assemble widgets

      pack .output.fr1 .output.fr2  -side top -fill both -expand true
      pack .output.fr1.scroll -side right -fill y
      pack .output.fr1.text -side left -fill both -expand true
      pack .output.fr2.close .output.fr2.saveAs\
         -side left -padx 50 -pady 5
      pack .output.fr2 -expand false
   } else {
      wm deiconify .output
   }
}

#############################################################################
#                       this is the browser window                          #
#############################################################################

proc showTableChoice {init} {
   global ks_dir tmp_dir

   if { $init=="firstTime"} {
      toplevel .tableChoice -bg #00d090
      wm withdraw .tableChoice
      wm geometry .tableChoice 600x300+120+140
      wm title .tableChoice Browser
      
      bind .tableChoice <Enter> {focus %W}
      
      global numberAltKnots numberNonaltKnots knotType nCrossings selectMode
      
      foreach item {{3 1} {4 1} {5 2} {6 3} {7 7} {8 18} {9 41} {10 123}\
                    {11 367} {12 1288} {13 4878} {14 19536} {15 85263}\
                    {16 379799}} {
         set numberAltKnots([lindex $item 0]) [lindex $item 1]
      }
      
      foreach item {{3 0} {4 0} {5 0} {6 0} {7 0} {8 3} {9 8} {10 42}\
                    {11 185} {12 888} {13 5110} {14 27436} {15 168030}\
                    {16 1008906}} {
         set numberNonaltKnots([lindex $item 0]) [lindex $item 1]
      } 
      
      foreach item {3 4 5 6 7} {set numberNonaltKnots($item) 0} 
           
      foreach item {1 2 3 4} {
         frame .tableChoice.fr$item -bg #00d090
      }
      
      foreach item {{1 3} {1 4} {1 5} {1 6} {2 7} {2 8} {2 9} {2 10}\
             {3 11} {3 12} {3 13} {3 14} {4 15} {4 16} {4 17} {4 18}} {
         set a [lindex $item 0]
         set b [lindex $item 1]
         radiobutton .tableChoice.fr$a.cr$b -text "$b crossings"\
            -variable nCrossings -value $b -width 13 -anchor w\
            -bg #00d090 -selectcolor yellow\
            -activebackground #00eeff -bd 2\
            -font "-adobe-new century schoolbook-bold-r-*-*-14-*-*-*-*-*-*-*"\
	    -command initKnotNumbers
      }
      
      set knotType a
      set nCrossings 8
      set selectMode s
      
      
      foreach item {1 2 3 4} {
         pack .tableChoice.fr$item -side top
      }
      
      foreach item {{1 3} {1 4} {1 5} {1 6} {2 7} {2 8} {2 9} {2 10}\
             {3 11} {3 12} {3 13} {3 14} {4 15} {4 16}} {
         set a [lindex $item 0]
         set b [lindex $item 1]
         pack .tableChoice.fr$a.cr$b -side left -anchor w -padx 6 -pady 5      
      }
      
#      pack .tableChoice.fr4 -side top -anchor w
#      pack .tableChoice.fr4.cr15 -side left -padx 15 -pady 5
#      pack .tableChoice.fr4.cr16 -side left -padx 3 -pady 5

      
      frame .tableChoice.fr5 -bg #00d090
      frame .tableChoice.fr6 -bg #00d090     
      frame .tableChoice.fr7 -bg #00d090  
          
      radiobutton .tableChoice.fr5.alt -bg #00d090 -text "Alternating"\
         -variable knotType -value "a" -anchor w -width 11\
         -activebackground #00eeff -command initKnotNumbers
      radiobutton .tableChoice.fr5.nonalt -bg #00d090 -text Non-alternating\
         -variable knotType -value "n" -anchor w -width 15 \
         -activebackground #00eeff -command initKnotNumbers
      label .tableChoice.fr5.label -text "Select knot #" -width 14 -bg #00d090\
         -anchor w
      entry .tableChoice.fr5.entry -width 8 -textvariable firstKnot\
         -highlightbackground #00d090 -highlightcolor yellow\
         -bg #00d0ff
      label .tableChoice.label -text "Finish at knot #" -width 14 -bg #00d090\
         -anchor w
      entry .tableChoice.entry -width 8 -textvariable lastKnot\
         -highlightbackground #00d090 -highlightcolor yellow\
         -highlightthickness 2 -bg #00d0ff
         
      radiobutton .tableChoice.fr6.single -bg #00d090 -text "Single Knot"\
         -variable selectMode -value "s" -command "setSelectMode; initKnotNumbers"\
         -activebackground #00eeff -width 11 -anchor w
      radiobutton .tableChoice.fr6.range -bg #00d090 -text "Range of Knots"\
         -variable selectMode -value "r" -command "setSelectMode; initKnotNumbers"\
         -activebackground #00eeff -width 15 -anchor w

	 
      bind .tableChoice <s> {if {$selectMode == {r}} {
         set selectMode "s"
	 initKnotNumbers
	 setSelectMode
      } else {
         set selectMode "r"
	 initKnotNumbers
	 setSelectMode
      }}
      bind .tableChoice <a> {if {$knotType == {n}} {
         set knotType "a"
	 initKnotNumbers
      } else {
         set knotType "n"
	 initKnotNumbers
      }}
      
      bind .tableChoice <Left> {if {$nCrossings > 3} {incr nCrossings -1; initKnotNumbers}}
      bind .tableChoice <Right> {if {$nCrossings < 15} {incr nCrossings 1; initKnotNumbers}}
      bind .tableChoice <Up> {if {$nCrossings > 6} {incr nCrossings -4; initKnotNumbers}}
      bind .tableChoice <Down> {if {$nCrossings < 12} {incr nCrossings 4; initKnotNumbers}}

  
      button .tableChoice.fr7.load -text Load -bg #0090ff\
         -activebackground #00eeff -highlightthickness 2 -highlightbackground yellow\
         -command {getFromTable $nCrossings $knotType $firstKnot $lastKnot 1}
      button .tableChoice.fr7.append -text Append -bg #0090ff\
         -activebackground #00eeff\
         -command {getFromTable $nCrossings $knotType $firstKnot $lastKnot 2}
      button .tableChoice.fr7.dismiss -text Dismiss -bg #0090ff\
         -activebackground #00eeff -command {wm withdraw .tableChoice}
      
      bind .tableChoice <Return> {getFromTable $nCrossings $knotType $firstKnot $lastKnot 1}
      bind .tableChoice <Control-a> {getFromTable $nCrossings $knotType $firstKnot $lastKnot 2}
      bind .tableChoice <Control-q> {wm withdraw .tableChoice}
      
      pack .tableChoice.fr5 .tableChoice.fr6 .tableChoice.fr7\
         -side top -expand true -fill both
         
      pack .tableChoice.fr5.label .tableChoice.fr5.entry\
         -side left -anchor se -padx 9 -pady 10
      pack .tableChoice.fr5.nonalt .tableChoice.fr5.alt -side right -anchor sw\
         -padx 10 -pady 10

      pack .tableChoice.label -side left -anchor se\
         -padx 9 -pady 10 -in .tableChoice.fr6
      pack .tableChoice.entry -side left -anchor se\
         -padx 9 -pady 10 -in .tableChoice.fr6               
      pack .tableChoice.fr6.range -side right -anchor sw\
         -padx 10 -pady 10
      pack .tableChoice.fr6.single -side right -anchor sw\
         -padx 10 -pady 10
             
      lower .tableChoice.label
      lower .tableChoice.entry
                  
      pack .tableChoice.fr7.load .tableChoice.fr7.append .tableChoice.fr7.dismiss\
         -side left -padx 50 -pady 10

              
   } else {
      initKnotNumbers
      wm deiconify .tableChoice
   }

   proc setSelectMode {} {
      global selectMode
      
      if {$selectMode == "s"} {
         lower .tableChoice.label
         lower .tableChoice.entry
         .tableChoice.fr5.label configure -text "Select knot #"
      } else {
         raise .tableChoice.label .tableChoice.fr6
         raise .tableChoice.entry
         .tableChoice.entry configure\
            -highlightbackground #00d090 -highlightcolor yellow
         .tableChoice.fr5.label configure -text "Start at knot #"        
      }      
   }   
   
   proc initKnotNumbers {} {
      global knotType numberAltKnots numberNonaltKnots nCrossings selectMode
      
      .tableChoice.fr5.entry delete 0 end
      .tableChoice.entry delete 0 end 
      
#      if {$nCrossings > 130} { 
#         if {$selectMode == "r"} {
#            set str1 "There are too many knots with $nCrossings crossings"
#            set str2 "to load all at once.  Please use the entry fields"
#            set str3 "to choose a range containing up to 10000 knots."
#            set str4 "For certain \"actions\" there are further limitations:"
#            set str5 "See Help / Operational Limits for details."
#            set str [concat $str1 $str2 $str3 $str4 $str5]
#            infoPopup $str 200 0
#         }
#         return
#      }
#      
#      if {$nCrossings < 8 && $knotType == "n"} {
#         set str1 "There are no non-alternating knots with"
#         set str2 "$nCrossings crossings."
#         set str [concat $str1 $str2]
#         infoPopup $str 200 0
#      }
                
      .tableChoice.fr5.entry insert 0 1
      if {$knotType == "a"} {
         .tableChoice.entry insert 0 $numberAltKnots($nCrossings)
      } else {
         .tableChoice.entry insert 0 $numberNonaltKnots($nCrossings)
      }
   }


   proc getFromTable {nCrossings knotType firstKnot lastKnot mode} {
   
      global ks_dir tmp_dir selectMode
      
      if {$selectMode == "s"} {set lastKnot $firstKnot}
      
#      if {[expr $lastKnot - $firstKnot] > 9999} {
#         infoPopup "Too many knots have been selected (limit 10000)." 200 0
#         return
#      }


      if {$nCrossings < 8 && $knotType == "n"} {
         set str1 "There are no non-alternating knots with"
         set str2 "$nCrossings crossings."
         set str [concat $str1 $str2]
         infoPopup $str 200 0
	 return
      }
      
      
      if {[catch {exec $ks_dir/bin/decode_$knotType $ks_dir/knotTable/$nCrossings$knotType.pak\
            $tmp_dir/K[pid]knotsel $firstKnot $lastKnot}] == 1} {
         warning "An error occurred in decoding the knot table"
         return
      }
      
      writeToWindow .input $mode $tmp_dir/K[pid]knotsel 
   }   
}

############################################################################
#            procedure for finding user-supplied knot in table             #
############################################################################

#this reads knot from input, reduces and looks up in table

proc locateInTable {} {

   global ks_dir tmp_dir

#   if {$init == "firstTime"} return
   
# read knot from selection, or from first line of input text widget if
# nothing is selected

   if {[string length [.input.fr1.text tag ranges sel]] == 0} {
      set selectedKnots [.input.fr1.text get 1.0 1.end]
   } else {
      set selectedKnots [.input.fr1.text get sel.first sel.last]
   }
  
   set f0 [open $tmp_dir/K[pid]knotfile0 w]
   puts $f0 $selectedKnots
   close $f0
   
   set f0 [open $tmp_dir/K[pid]knotfile0 r]

   warning2 "Working ..." 400 80
   update idletasks
   set f 0

   while {[gets $f0 str] >= 0} {

      set str [string trim $str]
      if {[string length $str] == 0} continue

      set inputList [list $str]

# check integrity of data

      set str [string trim $str]

      if {[checkIntegrity $str] == 1} {
         if {[winfo exists .warning2]} {destroy .warning2}
         return
      }

      set outputList ""
  
      while {[llength $inputList] > 0} {

# Here's the algorithm:
# Always apply knotfind to 0th diagram in inputList
# Three possible outcomes to knotfind:
#   (i)  unknot,  (ii)  prime,  (iii)  connected sum
# In case (i) simply erase from inputList
# In case (ii) transfer to outputList
# In case (iii) replace 0th diagram by two summands

         set f [open $tmp_dir/K[pid]knotfile w]
         puts $f [lindex $inputList 0]
         close $f

         if {[catch {exec $ks_dir/bin/knotfind $tmp_dir/K[pid]knotfile $tmp_dir/K[pid]knotfind.out}] == 1} {
            destroy .warning2
            warning "An error has occurred in the locate function"
            return
         }
         set f [open $tmp_dir/K[pid]knotfind.out r]
         set result [gets $f]
         close $f

         set tmpList [lreplace $inputList 0 0]
         set inputList $tmpList
# case (i)         
         if {$result == "unknot"} {continue}
# case (ii)         
         if {[llength $result] == 1} {
            lappend outputList [join $result]
         } else {
# case (iii)
            set tmpList [concat $inputList $result]
            set inputList $tmpList
         }
      }
 
      if {[llength $outputList] == 0} {
         .output.fr1.text insert end "unknot\n"
         wm deiconify .output
         continue
      }
    
      if {[llength $outputList] > 1} {
         .output.fr1.text insert end\
         "\n********************************\nComposite knot, with factors\n"
      }
 
      update idletasks
      
      set outputListPosition 0

      foreach item $outputList {
 
         incr outputListPosition 1

         set x [string first - $item]
         if {$x == -1} {
            set knotType a
         } else {
            set knotType n
         }
 
         set nCrossings [lindex $item 0]
 
#    if nCrossings > 16, just record reduced DT sequences

         if {$nCrossings > 16} {
            set noOfCrossings [lindex $item 0]
            set f [open $tmp_dir/K[pid]best_available w]
            
            puts -nonewline $f [format "%2d%13d    " $noOfCrossings  1  ]
            for {set i 1} {$i <= $noOfCrossings} {incr i 1} {
               set term [lindex $item $i]
               puts -nonewline $f [format "%4d" $term]
            }
            puts -nonewline $f "  (best available reduction)\n"
            close $f
            
            writeToWindow .output 2 $tmp_dir/K[pid]best_available

            wm deiconify .output
            continue
         }

#    if nCrossings = 14, 15 or 16, check knot in special list of duplicates
 
         if {$nCrossings >= 14 && $nCrossings <= 16 && $knotType == "n"} {
            set f [open $tmp_dir/K[pid]knotfind.tmp w]
            puts $f $item
            close $f
            
            if {[catch {exec $ks_dir/bin/dupsearch $tmp_dir/K[pid]knotfind.tmp $ks_dir/knotTable/duplicates\
                           $tmp_dir/K[pid]knotfind.out}] == 1} {
               destroy .warning2
               warning "An error has occurred in reading the table of duplicates"
               return
            }                           
                           
            set f [open $tmp_dir/K[pid]knotfind.out r]
            set item1 [gets $f]
            close $f
            
            if {[string length $item1] > 0} {
               .output.fr1.text insert end $item1
               wm deiconify .output
               continue
            }
         }

         set f [open $tmp_dir/K[pid]knotfind.tmp w]

         puts $f $item
         close $f
         
         if {[catch {exec $ks_dir/bin/locate_$knotType $ks_dir/knotTable/$nCrossings$knotType.pak\
            $tmp_dir/K[pid]knotfind.tmp $tmp_dir/K[pid]lookup.output}] == 1} {
            destroy .warning2
            warning "An error has occurred while attempting to read the knot table"
            return
         }
 
         if {$nCrossings < 10} {
            set ncstr " $nCrossings"
         } else {
            set ncstr "$nCrossings"
         }
         
         .output.fr1.text insert end $ncstr end "  "\
            end $knotType end "  "
 
         writeToWindow .output 3 $tmp_dir/K[pid]lookup.output
         
         if {[llength $outputList] > 1 && [llength $outputList] == $outputListPosition} {
            .output.fr1.text insert end\
            "********************************\n\n"
         }
         update idletasks
      }

      .output.fr1.text insert end ""

   }
   
   destroy .warning2
   
   close $f0
}

############################################################################
#     procedure for finding homomorphisms by invoking knot_group_reps      #
############################################################################
 
#Only for S_5 at present.  Homology feature operational, though

proc showHomChoice {init} {
   
   global ks_dir tmp_dir homType wh1

   if {$init=="firstTime"} {
      toplevel .homChoice
      wm withdraw .homChoice
      wm geometry .homChoice 450x300+240+140
      
      set symgp 5
      set homType 1
      set wh1 0
      
      foreach item {1 2 3} {
         frame .homChoice.fr$item -bg #ff9bee
         pack .homChoice.fr$item -side left -fill both -expand true
      }
      
      foreach item {{1 "cycle type"} {2 options} {3 action}} {
         set i1 [lindex $item 0]
         set i2 [lindex $item 1]
         
         label .homChoice.fr$i1.label -text $i2 -bg #ff9bee\
         -font "-adobe-new century schoolbook-medium-r-*-*-20-*-*-*-*-*-*-*"
         pack .homChoice.fr$i1.label -side top -padx 15 -pady 5 -anchor w
      }
      
      foreach item\
         {{(ab) 1} {(abc) 2} {(abcd) 3} {(abcde) 4} {(ab)(cd) 5} {(abc)(de) 6}} {
          
         set i1 [lindex $item 0]
         set i2 [lindex $item 1]
         
         radiobutton .homChoice.fr1.$i2 -text $i1 -bg #a0e0ff -activebackground blue\
           -variable homType -value $i2 -relief raised -bd 2 -width 10 -anchor w
      }
      
      bind .homChoice <Up> {if {$homType >1} {incr homType -1}}
      bind .homChoice <Down> {if {$homType < 6} {incr homType 1}}
      
      checkbutton .homChoice.fr2.homology -text homology -variable wh1 -bg #ccaaff\
         -activebackground blue -relief raised -bd 2\
         -width 10
	 
      bind .homChoice <h> {if {$wh1 == 0} {set wh1 1} elseif {$wh1 == 1} {set wh1 0}}
	 
      radiobutton .homChoice.fr2.s5 -text "degree 5" -variable symgp -value 5 -bg #ccaaff\
         -activebackground blue -relief raised -bd 2\
         -width 10 
      radiobutton .homChoice.fr2.s6 -text "degree 6" -variable symgp -value 6 -bg #ccaaff\
         -activebackground blue -relief raised -bd 2\
         -width 10   
         
      button .homChoice.fr3.compute -text compute -bg green -activebackground red\
         -command {findHom $homType $symgp $wh1} -bd 3\
	 -highlightthickness 3.5 -highlightbackground #ff00aa
      button .homChoice.fr3.dismiss -text dismiss -bg #ee00bb -activebackground red\
         -command "wm withdraw .homChoice" -bd 3
      
      bind .homChoice <Return> {findHom $homType $symgp $wh1}
      bind .homChoice <Control-q> {wm withdraw .homChoice}
 
      foreach item {1 2 3 4 5 6} {
            pack .homChoice.fr1.$item -side top -padx 5 -pady 8 -anchor w
            
      }
      
      pack .homChoice.fr2.homology\
         -side top -anchor nw -padx 5 -pady 8 -expand true
      pack .homChoice.fr3.compute  -side top -anchor w\
         -padx 5 -pady 8 -expand true         
      pack .homChoice.fr3.dismiss  -side bottom -anchor w\
         -padx 5 -pady 8 -expand true
         
      proc findHom {homType symgp wh1} {
         
         global ks_dir tmp_dir
         
         set str2 "1"
         lappend str2 $homType $wh1
         
         set f [open $tmp_dir/K[pid]knottxt w]
 
         puts $f $str2 
 
 	 if {[string length [.input.fr1.text tag ranges sel]] == 0} {
 	   set knotData [.input.fr1.text get 1.0 end]
 	 } else {
 	   set knotData [.input.fr1.text get sel.first sel.last]
 	 }
 
	 puts $f $knotData
         close $f
           
         warning2 "Working ..." 400 80
         update idletasks
         
#         exec $ks_dir/bin/knot_group_reps $ks_dir/lib/hom_data $tmp_dir/K[pid]knottxt\
#            $tmp_dir/K[pid]rep1txt
            
         if {[catch {exec $ks_dir/bin/knot_group_reps $ks_dir/lib/hom_data $ks_dir/lib/cyclenotation\
            $tmp_dir/K[pid]knottxt $tmp_dir/K[pid]rep1txt}] != 0} {
            destroy .warning2
            warning "An error has occurred in the homomorphism procedure"
            return
         }

         destroy .warning2
         update idletasks
         
         writeToWindow .output 1 $tmp_dir/K[pid]rep1txt
	 
	 if {[.output.fr1.text get 1.0 1.end] == {}} {
	    .output.fr1.text insert end "No homomorphisms"
	 }
      }
 
   } else {
      wm deiconify .homChoice
   }
}


############################################################################
#            Procedure for computing determinant and signature             #
############################################################################

proc detSigCompute {} {
   
   global ks_dir tmp_dir
   
   set f [open $tmp_dir/K[pid]knottxt w]

   

   if {[string length [.input.fr1.text tag ranges sel]] == 0} {
      set knotData [.input.fr1.text get 1.0 end]
   } else {
      set knotData [.input.fr1.text get sel.first sel.last]
   }
   
   puts $f $knotData
   
   close $f
   
   if {[checkIntegrity $knotData] == 1} {return}
   
   update idletasks
   
   warning2 "Working ..." 400 80
   update idletasks   
      
   if {[catch {exec $ks_dir/bin/glsig $tmp_dir/K[pid]knottxt $tmp_dir/K[pid]detsig.out}] == 1} {
      destroy .warning2
      warning "An error has occurred in the computation of the determinant or signature"
      return
   }
   
   writeToWindow .output 1 $tmp_dir/K[pid]detsig.out
   
   destroy .warning2
   update idletasks
}


############################################################################
#        procedure for writing contents of a text window to a file.        #
############################################################################

#does it in batches of 1000lines

proc writeToFile {window file} {
   
   global ks_dir tmp_dir

   set f [open $file w]

   set str x
   set ln1 1
   set ln2 1001
   while {$str != {}} {
      set str1 [$window.fr1.text get $ln1.0 $ln2.0]
      set str [string trimright $str1]
      
      if {$str != ""} {
         puts $f $str
      }

      incr ln1 1000
      incr ln2 1000
   }
   
   close $f  
}


############################################################################
#             procedure for loading a file into a text window              #
############################################################################
   
proc writeToWindow {window mode file} {
   
   global ks_dir tmp_dir
   
   set windowType [string trimleft $window .]
   set str "Filename"
   set currentFilename "$windowType$str"
   
   global $currentFilename nBytes
   set nBytes 0

   if { [wm state $window]!="normal"} {
      wm deiconify $window
   }

#  If last character in text widget isn't a newline, add a newline

   set str1 [$window.fr1.text index "end -1 chars"]
   set periodIndex [string first "." $str1]
   set str1Length [string length $str1]
   set str2 [string range $str1 [expr $periodIndex+1] $str1Length]

   if {$str2 != "0" && $mode != 3} {
      $window.fr1.text insert end "\n"
   }
   
   if {$mode == 1} {
      $window.fr1.text delete 0.1 end
   }
   
   set f [open $file]
   while {![eof $f]} {
      set str1 [read $f 100]
      $window.fr1.text insert end $str1
      incr nBytes [string length $str1]      
   }
   close $f
   
   set $currentFilename $file
}

############################################################################
#                    polynomial computation procedure                      #
############################################################################

#procedure invokes Ewing-Millett polynomial programs

proc showPolyChoice {init} {
   
   global ks_dir tmp_dir polyType
   
   if { $init=="firstTime"} {
   
      toplevel .polynomials
      wm withdraw .polynomials
      wm geometry .polynomials 250x170+240+140
      
      bind .polynomials <Enter> {focus %W}
      bind Radiobutton <Return> {}

      frame .polynomials.fr1 -bg #f9c1ff
      frame .polynomials.fr2 -bg blue
      
      set polyType 2
      
      radiobutton .polynomials.fr1.alex -text "Alexander Polynomial" -bg #f9c1ff\
         -variable polyType -value 1 -anchor w\
         -activebackground #ff70ff
      radiobutton .polynomials.fr1.jones -text "Jones Polynomial" -bg #f9c1ff\
         -variable polyType -value 2 -anchor w\
         -activebackground #ff70ff
      radiobutton .polynomials.fr1.homfly -text "Homfly Polynomial" -bg #f9c1ff\
         -variable polyType -value 3 -anchor w\
         -activebackground #ff70ff
      radiobutton .polynomials.fr1.kauffman -text "Kauffman Polynomial" -bg #f9c1ff\
         -variable polyType -value 4 -anchor w\
         -activebackground #ff70ff
         
      button .polynomials.fr2.compute -text Compute\
         -command {polyCompute $polyType} -bg #44dd44 -activebackground green\
	 -highlightthickness 2.5 -highlightbackground #ff00aa
      button .polynomials.fr2.dismiss -text Dismiss -command "wm withdraw .polynomials"\
         -bg #ee5555 -activebackground red
      
      bind .polynomials <Control-q> {wm withdraw .polynomials}
      bind .polynomials <Down> {if {$polyType < 4} {incr polyType 1}}
      bind .polynomials <Up> {if {$polyType > 1} {incr polyType -1}}
      bind .polynomials <Return> {polyCompute $polyType}

      pack .polynomials.fr1 .polynomials.fr2 -side top -fill both -expand true
      pack .polynomials.fr1.alex .polynomials.fr1.jones\
         .polynomials.fr1.homfly .polynomials.fr1.kauffman -side top -fill x -anchor w
 
      pack .polynomials.fr2.compute .polynomials.fr2.dismiss -side left -padx 5m -pady 2m 
      
   } else {
      wm deiconify .polynomials
   }

   proc polyCompute {polyType} {
      
      global ks_dir tmp_dir
      global alex conway jones homfly kauffman
      global outToFile outToWindow

      set f [open $tmp_dir/K[pid]mbt w]

      if {[string length [.input.fr1.text tag ranges sel]] == 0} {
        set knotData [.input.fr1.text get 1.0 end]
      } else {
        set knotData [.input.fr1.text get sel.first sel.last]
      }

      if {[checkIntegrity $knotData] == 1} {return}


      puts $f $knotData      

      close $f   
      
      update idletasks
      
         warning2 "Working ..." 305 80
         update idletasks
         
         if {[catch {exec $ks_dir/bin/mbt_ken $tmp_dir/K[pid]mbt $tmp_dir/K[pid]ken\
            $tmp_dir/K[pid]exitstate}] == 1} {
            destroy .warning2
            warning "An error has occurred while translating DT format to Ewing-Millett format"
            return
         }
         
         set f [open $tmp_dir/K[pid]exitstate r]
         set state [read $f]
         close $f
         
         if {$state == 1} {
            warning "input became unreadable"
            destroy .warning2
            return
         }      
   
      if {$polyType <= 3} {
              
         if {[catch {exec $ks_dir/bin/poly1 $tmp_dir/K[pid]ken $tmp_dir/K[pid]alex.txt\
            $tmp_dir/K[pid]jones.txt $tmp_dir/K[pid]homfly.txt}] == 1} {
            destroy .warning2
            warning "An error has occurred in the computation of polynomials"
            return
         }
          
         destroy .warning2 
         update idletasks
      
         if {$polyType == 1} {
            writeToWindow .output 1 $tmp_dir/K[pid]alex.txt
         }
      
         if {$polyType == 2} {
            writeToWindow .output 1 $tmp_dir/K[pid]jones.txt
         }
      
         if {$polyType == 3} {
            writeToWindow .output 1 $tmp_dir/K[pid]homfly.txt
         } 
      }
      
      if {$polyType == 4} {
         
         if {[catch {exec $ks_dir/bin/poly2 $tmp_dir/K[pid]ken $tmp_dir/K[pid]kauffman.txt}] == 1} {
            destroy .warning2
            warning "An error has occurred in the computation of polynomials"
            return
         }
           
         destroy .warning2 
         update idletasks
                            
         writeToWindow .output 1 $tmp_dir/K[pid]kauffman.txt
      }
   }
}

#create file selection dialog box on startup

proc openFile {window} {
   
   global ks_dir tmp_dir
   
   set file [fileselect "Select File" "$ks_dir/ "]
   
   if {[string length $file] > 0} {writeToWindow $window 1 $file}
}

proc saveFile {window} {
   global inputFilename ks_dir tmp_dir
   
   if {$inputFilename == "None"} {
      saveAs .input
      return 1
   }
   
   update idletasks
   
   exec cp $inputFilename $inputFilename.bak
   set f [open $inputFilename w]
   puts $f [$window.fr1.text get 1.0 end]
   close $f
}

proc saveAs {window} {

   global ks_dir tmp_dir
   
   set file [fileselect "Save As ..." "$ks_dir/ "]

   if {[string trim $file] != ""} {writeToFile $window $file}
}


# check integrity of knot data

proc checkIntegrity {data} {
  
  global ks_dir tmp_dir
  
  set f [open $tmp_dir/K[pid]checkknot.in w]
  puts -nonewline $f $data
  close $f
  
  if {[catch {exec $ks_dir/bin/checkknot $tmp_dir/K[pid]checkknot.in $tmp_dir/K[pid]checkknot.out\
       $tmp_dir/K[pid]kt}] == 1} {
     warning "An error has occurred in checking the integrity of the input data"
     return 1
  }
  
  set f [open $tmp_dir/K[pid]checkknot.out r]
  set str1 [read $f]

  if {$str1 != "OK\n"} {
     set str2 "Please refer to Help for correct format."
     set str [concat $str1 $str2]  
     warning $str
     return 1
  }
  close $f
  
  return 0
}

proc infoPopup {infoMessage xcoord ycoord} {
   toplevel .info
   wm transient .info .
   wm overrideredirect .info 1      
   wm geometry .info +$xcoord+$ycoord
   frame .info.top -relief raised -bd 1 -bg #c080ff
   frame .info.bottom -relief raised -bd 1 -bg #c080ff
   pack .info.top .info.bottom -side top -fill both -expand true
   message .info.top.msg -text "$infoMessage" -bg #c080ff\
   -font 9x15bold -width 15c -justify center
   pack .info.top.msg -side right -expand 1 -fill both -padx 3m -pady 3m
   button .info.bottom.ok -text "OK" -bg blue -command "destroy .info"\
      -activebackground #0000ff
   pack .info.bottom.ok -pady 3m
   update idletasks
   grab set .info
   tkwait window .info
}

proc warning {warningMessage} {
   toplevel .warning
   wm transient .warning .
   wm overrideredirect .warning 1
   wm geometry .warning +400+200
   frame .warning.top -relief raised -bd 1 -bg #d090ff
   frame .warning.bottom -relief raised -bd 1 -bg #d090ff
   pack .warning.top .warning.bottom -side top -fill both -expand true
   message .warning.top.msg -width 8c -text "$warningMessage" -bg #d090ff -justify center\
   -fg #a00000 -font "-adobe-new century schoolbook-medium-r-*-*-20-*-*-*-*-*-*-*"
   pack .warning.top.msg -side right -expand 1 -fill both -padx 3m -pady 3m
   button .warning.bottom.ok -text "OK" -bg blue -command "destroy .warning"\
      -activebackground #0000ff
   pack .warning.bottom.ok -pady 3m
   update idletasks
   grab set .warning
   tkwait window .warning
}

proc warning2 {warningMessage xcoord ycoord} {
   toplevel .warning2
   wm transient .warning2 .
   wm overrideredirect .warning2 1
   wm geometry .warning2 +$xcoord+$ycoord
   frame .warning2.top -relief raised -bd 1 -bg #c080ff
   frame .warning2.bottom -relief raised -bd 1 -bg #c080ff
   pack .warning2.top .warning2.bottom -side top -fill both -expand true
   message .warning2.top.msg -width 8c -text "$warningMessage" -fg yellow\
      -bg #c080ff -font 9x15bold
   pack .warning2.top.msg -side right -expand 1 -fill both -padx 3m -pady 3m
}

proc tidyExit {} {
   global tmp_dir
   
   foreach file [glob -nocomplain $tmp_dir/K[pid]*] {
      exec rm $file
   }
   
   exit
}













