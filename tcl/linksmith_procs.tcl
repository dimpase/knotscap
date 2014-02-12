# LinkSmith version 1.01
# 3/19/99
# Jim Hoste, Pitzer College, 1050 N Mills Ave, Claremont, CA 91711
# jhoste@pitzer.edu
# Morwen Thistlethwaite, Math Dept, Univ Tennessee Knoxville, Knoxville, TN 37916
# morwen@math.utk.edu

#---------------------------------------------------------------#
#                      Recent Changes                           #
#---------------------------------------------------------------#
#  3-19-99 Added Export menu, with two new commands:
#     Save Millett Code and Save Triangulation
#     The first will compute millett code, for knots or links,
#     and dump the output to a temp file. It will then ask user
#     for name of file to store it in and copy the temp file to the
#     user-named file. Diagrams saved in this way are given
#     consecutive ID's starting with 1. (This restarts with each new
#     session of LinkSmith.)
#
#     Changed program so that gaussCode is carried along as you 
#     draw and edit a diagram, but DT code is only computed when
#     you submit a knot to knotscape input window.
#     When saving a diagram, Gauss and DT code are no longer written
#     to the file.
#
#     The second procedure, Save Triangulation, will save 
#     SnapPea-compatible triangulation of a knot or link exterior
#     to a file named by the user.
#
#  submissions to knotscape are now given consecutive integer ID's
#     starting at 1. Note that these ID's may still coincide with ID's
#     of knots entered in the input window in other ways
#
#  changed edge format so that the ID is constant
#     and a separate canvas tag is included in position 9
#     and width in postion 10
#
#  eliminated selectedEdgeIDs list. Selected edges are now identified
#     by their width of 4
#
#  added edgeIndex( ) array defined as
#     edgeIndex(i) is the position of edge i in the list $edges
#
#  Removed old stuff that was improved by Morwen and commented out by him
#
#  Morwen made a number of changes:
#      Adjustment of widget colors: "lightblue" replaced by #85bdff, activebackground added.
#      Colors also added to menu widgets.
#      showHelp now takes an argument, enabling immediate display of desired help page.
#      Menubuttons spaced out slightly more, and text padded to make them of roughly equal widths.
#      "Quit" command replaced by "Quit LinkSmith".
#      .linksmith toplevel window's initial placement moved slightly so as not to obscure
#         Knotscape's input window completely.
#      Removed size specification of .linksmith so that canvas fits exactly.
#         (did the same with the output window, which hopefully will cure the
#         vanishing buttons phenomenon.)
#      Commented out "copy" and "paste" options from menu, as they aren't yet operable.
#      Noticed bug: error message occurs when "Cancel" is clicked in file selector.  Fixed.
#      Noticed bug: error message occurs when a file with extension other than .lsd is selected;
#         this was fixed by inserting "update idletasks" in the warning procedure.
#      Noticed bug: after failure to load because of non-"lsd" extension, LinkSmith
#         insists that diagram be "closed" before new attempt at loading.  Fixed.
#      Noticed bug: error message occurs when attempting to load twistdbl.lsd, and most
#         of the other files in the lsd directory.  Files appear to be incompatible somehow
#         with LinkSmith, so problem was fixed by deleting the files.
#      Adjusted backup prodedure: copied files to subdirectory "bak", with ".bak" inserted
#         in filename before .lsd .  Files already of form *.bak.lsd are saved, but not
#         backed up.  Subsequent backups overwrite backup file.
#      Improved filedialog procedure.  Dialog now appears with listing in correct directory.
#         The listing still won't list just the files with a certain extension.  Not sure
#         whether this is a bug or simply a non-feature.  At any rate, it probably wouldn't
#         be too hard to fix.
#      Put "Close" and "Quit LinkSmith" options into cascade submenus (not sure whether this
#         is best, but means that user can quit with a single mouse action, and retains the
#         options of saving or not saving.)



#---------------------------------------------------------------#
#                    Bugs that need to be fixed                 #
#---------------------------------------------------------------#

# None currently known!!!!

#---------------------------------------------------------------#
#               Description of Data Structures                  #
#---------------------------------------------------------------#
#  1. Edges
#     The edges are stored in a list named $edges, which is a
#     widely accessed global variable. Each element of $edges
#     is a list containing the data for a single edge and has the format
#
#     ID a b c d color prev next compID tag width
#  
#     where ID is an integer identifier
#     (a, b) and (c,d) are the coordinates of the endpoints
#     The orientation of an edge is always from (a,b) to (c,d)
#     color is the color of the edge
#     prev is the edgeID of the edge (if any) that connects to (a,b)
#     next is the edgeID of the edge (if any) that connects to (c,d)
#     if there is no prev or next edge than these variables are -1
#     compID is an integer that identifies all edges that are part of the same component
#     compID's need not be consecutive integers 
#     tag is the canvas drawing tag of this line segment
#     width is the width of the edge, either 1 or 4; selected edges are 4
#
#  1.1	 Related to the edge data structure is an array, edgeIndex(n), that
#     equals the index of edge n in the list $edges. This eliminates the
#     ubiquitous task of finding an edge in the list of edges every time
#     we need to change it, or look at its data.
#
#  2. Crossings
#     The crossings are stored in a list named $crossings
#     which is a widely accessed global variable. Each element
#     of crossings is a list corresponding to a single crossing
#     and has format
#
#     ID x y edgeID1 t color1 {a b c d} edgeID2 s color2 {e f g h} onTop arcID diskID sign
# 
#     ID is an integer identifiying the crossing
#     x and y are the coordinates of the crossing
#     edgeID1 is the edge ID of the first edge involved in the crossing
#     t is the fraction of the way along that edge that the crossing occurs
#     color1 is the color of the first edge
#     (a,b) (c,d) are the endpoints of the tiny edge drawn in as the overcrossing
#     edgeID2 is the edge ID of the second edge involved in the crossing
#     s is the fraction of the way along the second edge that the crossing occurs
#     color2 is the color of the second edge
#     (e,f) (g,h) are the endpoints of the tiny edge drawn as the overcrossing
#     onTop is either "firstOnTop" or "secondOnTop" as is the case
#     arcID is the cavas tag of the tiny edge that is the overcrossing
#     diskID is the canvas tag of the white disk drawn to hide the undercrossing
#     sign is +1 or -1 according as it's a right or left handed crossing
#
#  3. nearbyVertexInfo
#     When [nearVertex delta x y] is called it checks to see if any vertices
#     are within delta of the point (x,y). It returns 0, 1 or 2 for the valence
#     of the nearby vertex. (Zero means that there were no nearby vertices.)
#     In the case of a nearby vertex, data on that vertex is stored in the list
#     nearbyVertexInfo, a widely accessed global variable that is formatted as
#
#     nearX nearY edge valence end
#
#     nearX and nearY are the coords of the nearby vertex
#     edge is the first edge in the list of edges that contains this vertex
#     valence is 1 or 2 (zero is not possible because in that case, this list is empty)
#     end is "start", "finish", or "none" depending on whether the vertex is at
#     the start of the edge, the end of the edge or valence-2

#source $ks_dir/tcl/knotscape_procs.tcl

##################################################################
#                       set up LinkSmith window                  #
##################################################################

proc showLinkSmith {init} {

   global  currentFilename ks_dir knotscapeSubmissionID millettExportID
   
   if {$init=="firstTime"} {
      toplevel .linksmith
      wm withdraw .linksmith
      wm geometry .linksmith +70+150
      wm title .linksmith "LinkSmith        untitled"


      # Create output window for displaying diagnostics
      # Useful for debugging when adding new features
      # to LinkSmith. Not intended to be used in final product
      
      toplevel .lsoutput -width 20c
      text .lsoutput.text -yscrollcommand ".lsoutput.scroll set"
      scrollbar .lsoutput.scroll -command ".lsoutput.text yview"
      pack .lsoutput.scroll -side right -fill y
      pack .lsoutput.text -side left -fill y
      
      # If .lsoutput window is wanted, comment out following line
        wm withdraw .lsoutput

      # set up menus

      frame .linksmith.mbar -relief raised -bd 2 -bg #85bdff
      menubutton .linksmith.mbar.file -text " File " -menu .linksmith.mbar.file.menu\
        -bg #85bdff -activebackground #bbffff
      menubutton .linksmith.mbar.edit -text " Edit " -menu .linksmith.mbar.edit.menu\
        -bg #85bdff -activebackground #bbffff
      menubutton .linksmith.mbar.action -text Action -menu .linksmith.mbar.action.menu\
        -bg #85bdff -activebackground #bbffff
      menubutton .linksmith.mbar.export -text Export -menu .linksmith.mbar.export.menu\
        -bg #85bdff -activebackground #bbffff
      button .linksmith.mbar.help -text " Help " -relief flat\
        -command {showHelp linksmith}\
        -bg #85bdff -activebackground #bbffff

      menu .linksmith.mbar.file.menu -tearoff 0 -background #d0d0ff -activebackground #00e0ff
      .linksmith.mbar.file.menu add command -label "0pen.." -command openDiagram
      .linksmith.mbar.file.menu add command -label "Save" -command saveDiagram
      .linksmith.mbar.file.menu add command -label "Save As.." -command saveAsDiagram
      .linksmith.mbar.file.menu add command -label "Submit to Knotscape" -command submitToKnotscape
      .linksmith.mbar.file.menu add cascade -label "Close"\
        -menu .linksmith.mbar.file.menu.closeOptions
      .linksmith.mbar.file.menu add cascade -label "Quit LinkSmith"\
        -menu .linksmith.mbar.file.menu.quitOptions
      
      menu .linksmith.mbar.file.menu.closeOptions -tearoff 0\
        -background #d0d0ff -activebackground #00e0ff
      menu .linksmith.mbar.file.menu.quitOptions -tearoff 0\
        -background #d0d0ff -activebackground #00e0ff
        
      .linksmith.mbar.file.menu.closeOptions add command -label "Save diagram"\
        -command {closeDiagram noquit save}
      .linksmith.mbar.file.menu.closeOptions add command -label "Don't save"\
        -command {closeDiagram noquit nosave}      

      .linksmith.mbar.file.menu.quitOptions add command -label "Save diagram"\
        -command {closeDiagram quit save}
      .linksmith.mbar.file.menu.quitOptions add command -label "Don't save"\
        -command {closeDiagram quit nosave}

      menu .linksmith.mbar.edit.menu -tearoff 0 -background #d0d0ff -activebackground #00e0ff
      .linksmith.mbar.edit.menu add command -label "Delete" -command {deleteselectedEdges "yes"}
#      .linksmith.mbar.edit.menu add command -label "Copy" -command copySelection
#      .linksmith.mbar.edit.menu add command -label "Paste" -command pasteSelection
      .linksmith.mbar.edit.menu add command -label "Clear" -command startOver

      menu .linksmith.mbar.action.menu -tearoff 0 -background #d0d0ff -activebackground #00e0ff
#      .linksmith.mbar.action.menu add command -label "Descend" -command {warning "Descend not yet operable"}
      .linksmith.mbar.action.menu add command -label "Alternate" -command alternate
      .linksmith.mbar.action.menu add command -label "Reflect" -command reflect
      .linksmith.mbar.action.menu add command -label "Reverse" -command reverseSelection
      .linksmith.mbar.action.menu add command -label "Reverse All" -command reverseAll

      menu .linksmith.mbar.export.menu -tearoff 0 -background #d0d0ff -activebackground #00e0ff
      .linksmith.mbar.export.menu add command -label "Save Millett Code" -command saveMillettCode
      .linksmith.mbar.export.menu add command -label "Save Triangulation" -command saveTriangulation

      pack .linksmith.mbar.file .linksmith.mbar.edit .linksmith.mbar.action .linksmith.mbar.export -side left -padx 10
      pack .linksmith.mbar.help -side right

      #set up canvas and pack all widgets
      canvas .linksmith.c -width 15c -height 15c -bd 0 -bg white
      pack .linksmith.mbar -side top -fill both
      pack .linksmith.c -side top -fill both -expand true
   
      #set up bindings
      bind .linksmith.c <Button-1> {mouseClick left %x %y}
      bind .linksmith.c <Button-2> {mouseClick center %x %y}
      bind .linksmith.c <ButtonRelease-2> {button2release %x %y}
      bind .linksmith.c <Button-3> {mouseClick right %x %y}
      bind .linksmith.c <Motion> {rubberband %x %y}
      focus .linksmith.c
      
      #set global variables
      initGlobalVariables
      set currentFilename "untitled"
      set knotscapeSubmissionID 1
      set millettExportID 1
   
   } else {
      wm deiconify .linksmith
   }
}

########################################################################
#     	             	procedures used by linksmith                   #
########################################################################

#*******************************************#
#          I/O procedures                   #
#*******************************************#

#mbt
proc closeDiagram {option1 option2} {
   global edges currentFilename
   
   if {$option2 == "save"} {
      if {($edges == {}) && ($currentFilename == "untitled")} {
         infoPopup "Canvas is empty" 350 250
         return
      } else {
# "1" means that "cancel" has been clicked in fileselect
         if {[saveDiagram] == 1} {return}
      }
   }
   
   eraseCanvas
   # erase canvas includes initGlobals
   set currentFilename "untitled"
   wm title .linksmith [format "LinkSmith        %s" $currentFilename]
   
   if {$option1 == "quit"} {wm withdraw .linksmith}
}
#mbt
#******************************************
proc printData { } {

# this procedure is intended only for debugging
# and prints to a top level output window that
# will not be used in the final version

   global edges crossings gaussCode DTcode edgeIndex
   
   .lsoutput.text insert end "edge indices:\n"
   foreach i [lsort [array names edgeIndex]] {
      .lsoutput.text insert end [list $i $edgeIndex($i)]
      .lsoutput.text insert end "\n"
   }
   .lsoutput.text insert end "list of crossings:\n"
   foreach i $crossings {
      .lsoutput.text insert end $i
      .lsoutput.text insert end "\n"
   }
   .lsoutput.text insert end "gauss code\n"
   foreach i $gaussCode {
     .lsoutput.text insert end $i
     .lsoutput.text insert end "\n"
   }
   .lsoutput.text insert end "DT code\n"
   .lsoutput.text insert end $DTcode
}
#******************************************
proc submitToKnotscape { } {
   global  tmp_dir DTcode knotscapeSubmissionID

   findDTcode
   if {$DTcode!={}} {
  
      # write DT code to file, $tmp_dir/ls.out

#mbt
#    Way in which data is output changed slightly: written one term at a time
#    so as to get uniform formatting

      set f [open $tmp_dir/K[pid]ls.out w]
      set noOfCrossings [llength $DTcode]
      puts -nonewline $f [format "%2d %8d   " $noOfCrossings $knotscapeSubmissionID]
      for {set i 0} {$i < $noOfCrossings} {incr i 1} {
         set term [lindex $DTcode $i]
         puts -nonewline $f [format "%4d" $term]
      }
      puts -nonewline $f "\n"
      close $f
      
      writeToWindow .input 2 $tmp_dir/K[pid]ls.out
#mbt      
      incr knotscapeSubmissionID
   } else {
      warning "Sorry, the present version of Knotscape only\
      	 accepts knots."
   }

}
#********************************************
proc openDiagram { } {
   global ks_dir currentFilename edges crossings\
    crossingID nextComponentID nextColor edgeIndex segmentID
   
   if {($edges!={}) || ($currentFilename!="untitled")} {
      warning "Please close existing diagram before opening another."
   } else {
      set currentFilename [fileselect "Select File" $ks_dir/lsd/*.lsd]
      if {[string length $currentFilename] == 0 } {
         set currentFilename "untitled"
         return
      } else {
      	 if {[string match *.lsd $currentFilename]} {
      	    wm title .linksmith [format "LinkSmith        %s" $currentFilename]
      	    set f [open $currentFilename r]
      	    gets $f edge
      	    set nextComponentID 1
      	    set segmentID 0
      	    while {$edge!=0} {
      	       set tag [.linksmith.c create line\
      	          [lindex $edge 1] [lindex $edge 2] [lindex $edge 3] [lindex $edge 4]\
      	          -fill [lindex $edge 5] -arrow last ]
      	       set edge [lreplace $edge 9 9 $tag]
      	       set edge [lreplace $edge 10 10 1]
      	       lappend edges $edge
      	       set edgeIndex([lindex $edge 0]) [expr [llength $edges]-1]
      	       if {[lindex $edge 8]==$nextComponentID} {
      	          incr nextComponentID
      	       }
      	       if {[lindex $edge 0]>=$segmentID} {
      	          set segmentID [expr [lindex $edge 0]+1]
      	       }
      	       if {[lindex $edge 5]==$nextColor} {
      	          advanceColor
      	       }
      	       gets $f edge
      	    }
      	    gets $f crossing
      	    set crossingID 1
    	    while {$crossing!=0} {
      	       set X [lindex $crossing 1]
      	       set Y [lindex $crossing 2]
      	       set diskID [.linksmith.c create oval [expr $X-4] [expr $Y-4] [expr $X+4] [expr $Y+4] \
	          -fill white -outline white]
	       set crossing [lreplace $crossing 13 13 $diskID]
      	       if {[lindex $crossing 11]=="firstOnTop"} {
      	          set crossingEdge [lindex $crossing 6]
      	          set color [lindex $crossing 5]
      	       } else {
      	          set crossingEdge [lindex $crossing 10]
      	          set color [lindex $crossing 9]
      	       }
      	       set crossingArcID [.linksmith.c create line [lindex $crossingEdge 0] [lindex $crossingEdge 1]\
      	          [lindex $crossingEdge 2] [lindex $crossingEdge 3]\
      	          -fill $color]
      	       set crossing [lreplace $crossing 12 12 $crossingArcID]  
      	       lappend crossings $crossing
      	       if {[lindex $crossing 0]==$crossingID} {
      	          incr crossingID
      	       }
      	       gets $f crossing
      	    }
      	    findGaussCode
#      	    printData
      	 } else {
      	    warning "Sorry, LinkSmith will only open files with a .lsd extension"
            set currentFilename "untitled"
      	 }
      } 
   }
}
#********************************************
proc writeDataToFile {fileName} {
   global ks_dir edges crossings  
   
   set f [open $fileName w]
   
   foreach edge $edges {
      puts $f $edge
   }
   puts $f 0
   foreach crossing $crossings {
      puts $f $crossing
   }
   puts $f 0
   close $f
}
#********************************************
proc saveAsDiagram { } {
   global ks_dir currentFilename
   
   
   set filename [fileselect "Save As ..." "$ks_dir/lsd/ "]
   if {$filename == ""} {return 1}
   
   writeDataToFile $filename
   
   if {![string match *.lsd $filename]} {
      set extension ".lsd"
      exec mv $filename $filename$extension
      set filename $filename$extension
   }
   if {$currentFilename=="untitled"} {
      set currentFilename $filename
      wm title .linksmith [format "LinkSmith        %s" $currentFilename]
   }
}
#********************************************
proc saveDiagram { } {

# Notes:
# If "untitled", use saveAsDiagram.  A return value of 1 means that "cancel" has been clicked.
# Only make backup if filename isn't of form *.bak.lsd.  Also,
# at most one backup is made; subsequent backups will overwrite backup file.

   global ks_dir currentFilename

   if {$currentFilename=="untitled"} {
      if {[saveAsDiagram] == 1} {return 1}
   } else {
      set currentFilenameTail0 [file tail $currentFilename]
# strip .lsd from file name.
      set currentFilenameTail1 [string trimright $currentFilenameTail0 .lsd]
      set nameLength [string length $currentFilenameTail1]
# extract last 4 characters of stripped file name.
      set strtmp [string range $currentFilenameTail1 [expr $nameLength - 4] $nameLength]
      if {$strtmp != ".bak"} {
         set backupDirectory $ks_dir/lsd/bak
         set backupFilename $backupDirectory/$currentFilenameTail1.bak.lsd
         exec cp $currentFilename $backupFilename
      }
      writeDataToFile $currentFilename
   }
}
#***************************************************************#
proc saveMillettCode { } {
   global gaussCode tmp_dir home_dir 
   
   if {$gaussCode!={}} {
      findMillettCode
      set millettFile [fileselect "Save As ..." "$home_dir/ "]
      exec cp $tmp_dir/K[pid]millett.out $millettFile
   } else {
      warning "Can only save knot or link diagram."
   }
}
#***************************************************************#
proc saveTriangulation { } {
   global gaussCode ks_dir tmp_dir home_dir
 
   if {$gaussCode!={}} {
      findMillettCode  
      set triangulationFile [fileselect "Save As ..." "$home_dir/ "]
      
      if {[catch {exec $ks_dir/bin/ken2snap $tmp_dir/K[pid]millett.out\
          $triangulationFile $tmp_dir/K[pid]millett.tmp}] == 1} {
        warning "An error occurred in saving the triangulation file"
        return
      }
   } else {
      warning "Can only save knot or link complement."   
   }
}
#*******************************************#
#     procedures handling mouse clicks      #
#*******************************************#
proc mouseClick {button x y} {
   global arcInProgress lastX lastY
   
   set nearbyVertexValence [nearVertex 5 $x $y]
   switch $button {
      left { 
      	 set nearbyEdge [nearEdge 1 $x $y]
      	 if {!$arcInProgress } {
      	    if {($nearbyVertexValence==1) || (($nearbyVertexValence==0) && ($nearbyEdge=={}))} {
      	       startPLarc $x $y
      	    }
      	 } else {
      	    if {[generalPosition $x $y $lastX $lastY]} {
      	       if {$nearbyVertexValence==1} {
      	          endPLarc $x $y
      	          findGaussCode
      	       } else {
      	          if {($nearbyEdge=={}) && ($nearbyVertexValence==0)} {
      	             continuePLarc $x $y
      	          } 
      	       }
      	    }
       	 }
      }
      center { 
      	 if {!$arcInProgress} {
      	    set nearbyCrossing [nearCrossing 10 $x $y]
      	    if {$nearbyCrossing!={}} {
      	       changeCrossing $nearbyCrossing
      	    } elseif { [nearVertex 5 $x $y]!=0} {
      	       initiateDragVertex
      	    } else {
      	       set nearbyEdge [nearEdge 1 $x $y]
      	       if {$nearbyEdge!={}} {
      	          selectEdge $nearbyEdge
      	       }
      	    }
      	 }
      }
      right { 
       	 set nearbyEdge [nearEdge 1 $x $y]
      	 if {$arcInProgress} {
      	    if {([generalPosition $x $y $lastX $lastY]) &&\
      	           (($nearbyVertexValence==1) || (($nearbyEdge=={}) && ($nearbyVertexValence==0)))} {
       	       endPLarc $x $y
      	       findGaussCode
      	    }
       	 } else {
       	    set nearbyCrossing [nearCrossing 10 $x $y]
      	    if {($nearbyCrossing=={}) && ($nearbyEdge!={})} {
      	       selectComponent $nearbyEdge
      	    }
      	 }
      }
   }
}
#******************************************
proc button2release {x y} {
   global vertexDragInProgress
   
   if {$vertexDragInProgress} {
      endDragVertex $x $y
      findGaussCode
   }

}
#******************************************
proc nearVertex {delta x y} {
   global edges nearbyVertexInfo
   
   foreach i $edges {
      if {[dist $x $y [lindex $i 1] [lindex $i 2]]< $delta} {
      	 set nearX [lindex $i 1]
      	 set nearY [lindex $i 2]
      	 set end "start"
      	 set valence [expr [lindex $i 6]==-1 ? 1 : 2]
      	 set nearbyVertexInfo [list $nearX $nearY $i $valence $end]
      	 return $valence  
      } else {
      	 if {[dist $x $y [lindex $i 3] [lindex $i 4]]< $delta} {
      	    set nearX [lindex $i 3]
      	    set nearY [lindex $i 4]
      	    set end "finish"
      	    set valence [expr [lindex $i 7]==-1 ? 1 : 2]
      	    set nearbyVertexInfo [list $nearX $nearY $i $valence $end]
      	    return $valence  
      	 }
      } 
   }
   set nearbyVertexInfo {}
   return 0
}
#******************************************
proc nearEdge {delta x y} {
   global edges 
   
   foreach i $edges {
      set length [dist [lindex $i 1] [lindex $i 2] [lindex $i 3] [lindex $i 4]]
      if {([dist $x $y [lindex $i 1] [lindex $i 2]])+ ([dist $x $y [lindex $i 3] [lindex $i 4]])\
      	 < ($length + $delta)} {
      	    return $i
      }
   }
   return {}
}
#******************************************
proc nearCrossing {delta x y} {
   global crossings 

   foreach i $crossings {
      if {[dist $x $y [lindex $i 1] [lindex $i 2]]< $delta} {
      	 return $i
      }
   }
   return {}
}
#*******************************************#
#     	    arc drawing procedures          #
#*******************************************#
proc rubberband {x y} {
   global arcInProgress vertexDragInProgress rubberEdgeID rubberEdgeID2\
      lastX lastY lastX2 lastY2  color
   
   # assumes rubberEdges were already created in either startPLarc\
   # or inititateDragVertex
   if {$arcInProgress || $vertexDragInProgress} {
      .linksmith.c delete $rubberEdgeID
      set rubberEdgeID [.linksmith.c create line $lastX $lastY $x $y -fill $color ]
      if {[info exists lastX2]} {
      	 .linksmith.c delete $rubberEdgeID2
      	 set rubberEdgeID2 [.linksmith.c create line $lastX2 $lastY2 $x $y -fill $color ]
      }
   }
}
#******************************************
proc startPLarc {x y} {
   global lastX lastY edges arcInProgress\
      color nextColor nearbyVertexInfo  end\
      prevEdge compID nextComponentID rubberEdgeID
   
   # we arrive here if user has clicked with left button
   # near a valence 1 vertex, or in open space, and
   # if arcInProgress is 0
    
   set valence [expr {($nearbyVertexInfo == {} ) ? 0: [lindex $nearbyVertexInfo 3] }]
   switch $valence {
      0 {
      	 set lastX $x
      	 set lastY $y
      	 set end finish
      	 set color $nextColor
      	 set prevEdge {} 
      	 set compID $nextComponentID
      	 incr nextComponentID
      }
      1 {
      	 set end [lindex $nearbyVertexInfo 4]
      	 set lastX [lindex $nearbyVertexInfo 0]
      	 set lastY [lindex $nearbyVertexInfo 1]
      	 set prevEdge [lindex $nearbyVertexInfo 2]
      	 set color [lindex $prevEdge 5]
      	 set compID [lindex $prevEdge 8]
      	 set nearbyVertexInfo {}
      }
   }
   set arcInProgress 1
   .linksmith.c config -cursor pencil
   set rubberEdgeID [.linksmith.c create line $x $y $x $y]
}
#******************************************
proc continuePLarc {x y} {
   global color edges prevEdge lastX lastY\
       end rubberEdgeID compID segmentID edgeIndex
       
    # we arrive here if user has clicked with left button
    # in white space, the proposed edge is in general position
    # and an arc is in progress
    
   .linksmith.c delete $rubberEdgeID
   if {$prevEdge!={}} {
      set prevEdgeID [lindex $prevEdge 0]
      set index $edgeIndex($prevEdgeID)
   } else {
      set prevEdgeID -1
   }
   if {$end=="finish"} {
      set tag [.linksmith.c create line $lastX $lastY $x $y -arrow last -fill  $color]
      set i [list $segmentID $lastX $lastY $x $y $color $prevEdgeID -1 $compID $tag 1]
      if { $prevEdgeID!=-1} {
      	 set prevEdge [lreplace $prevEdge 7 7 $segmentID]
      	 set edges [lreplace $edges $index $index $prevEdge]
      }
   } else {
      set tag [.linksmith.c create line $x $y $lastX $lastY -arrow last -fill  $color]
      set i [list $segmentID $x $y $lastX $lastY $color  -1 $prevEdgeID $compID $tag 1]
      if { $prevEdgeID!=-1} {
      	 set prevEdge [lreplace $prevEdge 6 6 $segmentID]
      	 set edges [lreplace $edges $index $index $prevEdge]
      }
   }
   set prevEdge $i
   lappend edges $i
   set edgeIndex([lindex $i 0]) [expr [llength $edges]-1]
   set lastX $x
   set lastY $y
   incr segmentID
   findCrossings
#  printData
}
#******************************************
proc endPLarc {x y} {
   global color edges lastX lastY arcInProgress\
   	noOfArcEnds  end nearbyVertexInfo\
   	rubberEdgeID prevEdge compID segmentID\
   	edgeIndex
   
   # we arrive here if user has clicked with right or left button
   # near a valence 1 vertex or in open space, and the proposed
   # edge is in general position
   # in particular, nearbyVertexInfo is either empty or 
   # contains information about a valence 1 vertex
   
   .linksmith.c config -cursor top_left_arrow  
   .linksmith.c delete $rubberEdgeID
   
   set valence [expr {($nearbyVertexInfo == {} ) ? 0: [lindex $nearbyVertexInfo 3] }]
   if {$valence==1} {
      set x [lindex $nearbyVertexInfo 0]
      set y [lindex $nearbyVertexInfo 1] 
      set nextEdge [lindex $nearbyVertexInfo 2]
      set nextEdgeID [lindex $nextEdge 0]
      set indexOfNextEdge $edgeIndex($nextEdgeID)
      set nextEdgeEnd [lindex $nearbyVertexInfo 4]
      set nearbyVertexInfo {}
   } else {
      set nextEdge {}
      set nextEdgeID -1
   }
      
   if {$prevEdge!={}} {
      set prevEdgeID [lindex $prevEdge 0]
      set indexOfPrevEdge $edgeIndex($prevEdgeID)
   } else {
      set prevEdgeID -1
   }
   
   switch $valence {
      0 {
      	 if {$end=="finish"} {
      	    set tag [.linksmith.c create line $lastX $lastY $x $y -arrow last -fill  $color]
      	    set j [list $segmentID $lastX $lastY $x $y $color $prevEdgeID -1 $compID $tag 1]
      	    if {$prevEdgeID!=-1} {
      	       set prevEdge [lreplace $prevEdge 7 7 $segmentID]
      	    }
      	 } else {
      	    set tag [.linksmith.c create line $x $y $lastX $lastY -arrow last -fill  $color]
      	    set j [list $segmentID $x $y $lastX $lastY $color -1 $prevEdgeID $compID $tag 1]
      	    if {$prevEdgeID!=-1} {
      	       set prevEdge [lreplace $prevEdge 6 6 $segmentID]
      	    }
      	 }
      	 lappend edges $j
      	 set edgeIndex([lindex $j 0]) [expr [llength $edges]-1]
      	 if {$prevEdgeID!=-1} {
      	    set edges [lreplace $edges $indexOfPrevEdge $indexOfPrevEdge $prevEdge]
      	 }
         findCrossings
         set arcInProgress 0
         advanceColor
         incr segmentID
#      	 printData
     }
     1 {
      	 if {$compID==[lindex $nextEdge 8]} {
      	    if {$end=="finish"} {
      	       set tag [.linksmith.c create line $lastX $lastY $x $y -arrow last -fill  $color]
      	       set j [list $segmentID $lastX $lastY $x $y $color $prevEdgeID $nextEdgeID $compID $tag 1]
      	       if {$prevEdgeID!=-1} {
      	          set prevEdge [lreplace $prevEdge 7 7 $segmentID]
      	       }
      	       set nextEdge [lreplace $nextEdge 6 6 $segmentID]
      	    } else {
      	       set tag [.linksmith.c create line $x $y $lastX $lastY -arrow last -fill  $color]
      	       set j [list $segmentID $x $y $lastX $lastY $color $nextEdgeID $prevEdgeID $compID $tag 1]
      	       if {$prevEdgeID!=-1} {
      	          set prevEdge [lreplace $prevEdge 6 6 $segmentID]
      	       }
      	       set nextEdge [lreplace $nextEdge 7 7 $segmentID]
      	    }
      	    lappend edges $j
      	    set edgeIndex([lindex $j 0]) [expr [llength $edges]-1]
      	    if {$prevEdgeID!=-1} {
      	       set edges [lreplace $edges $indexOfPrevEdge $indexOfPrevEdge $prevEdge]
      	    }
      	    set edges [lreplace $edges $indexOfNextEdge $indexOfNextEdge $nextEdge]
            findCrossings
            set arcInProgress 0
            advanceColor
            incr segmentID
#      	    printData
       	 } else {
      	    if {$end==$nextEdgeEnd} {
      	       redrawArc $nextEdge $color opposite 0 $compID
      	    } else {
      	       redrawArc $nextEdge $color same 0 $compID
      	    }
#      	    printData
      	    nearVertex 5 $x $y
      	    endPLarc $x $y
      	 }
      }
   }
}
#******************************************
proc redrawEdge {edge color direction width compID} {
   global edges crossings edgeIndex 
   
   # redraws the given edge with the given color, direction
   # width, and compID. direction can either be "same" or
   # "opposite". Typically, we'll use this procedure
   # if we want to reorient, recolor, or change the width
   # of an edge.
   
   # erase the edge
   .linksmith.c delete [lindex $edge 9]
   
   # redraw the edge
   switch $direction {
      same {
      	 set tag [.linksmith.c create line [lindex $edge 1] [lindex $edge 2]\
      	    [lindex $edge 3] [lindex $edge 4] -fill $color -arrow last -width $width]
      	 set newEdge [list [lindex $edge 0] [lindex $edge 1] [lindex $edge 2]\
      	    [lindex $edge 3] [lindex $edge 4] $color [lindex $edge 6]\
      	    [lindex $edge 7] $compID $tag $width]
      }
      opposite {
      	 set tag [.linksmith.c create line [lindex $edge 3] [lindex $edge 4]\
      	    [lindex $edge 1] [lindex $edge 2] -fill $color -arrow last -width $width]
      	 set newEdge [list [lindex $edge 0] [lindex $edge 3] [lindex $edge 4]\
      	    [lindex $edge 1] [lindex $edge 2] $color [lindex $edge 7]\
      	    [lindex $edge 6] $compID $tag $width]
      }
   }
   
   # insert new edge in the list of edges
   set index $edgeIndex([lindex $edge 0])
   set edges [lreplace $edges $index $index $newEdge]
   
   # redraw all crossings involving this edge
   # and change signs and distances to crossings if an edge is reversed
   
   for {set i 0} {$i<[llength $crossings]} {incr i} {
      set crossing [lindex $crossings $i]
      if {[lindex $crossing 3]==[lindex $edge 0]} {
      	 .linksmith.c delete [lindex $crossing 12]
      	 .linksmith.c raise [lindex $crossing 13]
      	    
      	 set crossing [lreplace $crossing 5 5 $color]
      	 if {[lindex $crossing 11]=="firstOnTop"} {
      	    set sixOrTen 6
      	    set crossArcWidth $width
      	    set crossArcColor $color
      	 } else {
      	    set sixOrTen 10
      	    set crossEdgeID [lindex $crossing 7]
      	    set crossArcWidth [lindex [lindex $edges $edgeIndex($crossEdgeID)] 10]
      	    set crossArcColor [lindex $crossing 9]
      	 }
      	 set crossingArc [lindex $crossing $sixOrTen]
      	 set crossingArcID [.linksmith.c create line\
            [lindex $crossingArc 0] [lindex $crossingArc 1] [lindex $crossingArc 2] [lindex $crossingArc 3]\
            -fill $crossArcColor -width $crossArcWidth]
      	 set crossing [lreplace $crossing 12 12 $crossingArcID]
      	 if {$direction=="opposite"} {
      	    set crossing [lreplace $crossing 4 4 [expr 1-[lindex $crossing 4]]]
      	    set crossing [lreplace $crossing 14 14 [expr -1*[lindex $crossing 14]]]
      	 }
      	 set crossings [lreplace $crossings $i $i $crossing]
      } else {	
      	 if {[lindex $crossing 7]==[lindex $edge 0]} {
      	    .linksmith.c delete [lindex $crossing 12]
      	    .linksmith.c raise [lindex $crossing 13]
      	    set crossing [lreplace $crossing 9 9 $color] 
      	    if {[lindex $crossing 11]=="firstOnTop"} {
      	       set sixOrTen 6
      	       set crossEdgeID [lindex $crossing 3]
      	       set crossArcWidth [lindex [lindex $edges $edgeIndex($crossEdgeID)]  10]
      	       set crossArcColor [lindex $crossing 5]
      	    } else {
      	       set sixOrTen 10
      	       set crossArcWidth $width
      	       set crossArcColor $color
      	    }
       	    set crossingArc [lindex $crossing $sixOrTen]
      	    set crossingArcID [.linksmith.c create line\
               [lindex $crossingArc 0] [lindex $crossingArc 1] [lindex $crossingArc 2] [lindex $crossingArc 3]\
               -fill $crossArcColor -width $crossArcWidth] 
      	    set crossing [lreplace $crossing 12 12 $crossingArcID] 
      	    if {$direction=="opposite"} {
      	       set crossing [lreplace $crossing 8 8 [expr 1-[lindex $crossing 8]]]
      	       set crossing [lreplace $crossing 14 14 [expr -1*[lindex $crossing 14]]]
      	    }
      	    set crossings [lreplace $crossings $i $i $crossing] 
      	 }
      }	
   }
}
#**************************************************
proc redrawComponent {edge color direction width compID} {
   global edges edgeIndex

   # if width is 0, then all edges will be redrawn with 
   # their orignal width. If width is nonzero, then
   # all edges will be redrawn with the new given width
   
   # remember IDs of edges front and back of given edge
   set nextID [lindex $edge 7]
   set prevID [lindex $edge 6]
   
   # redraw the given edge
   redrawEdge $edge $color $direction [expr {($width==0) ? [lindex $edge 10]: $width}] $compID
   
   # redraw from the given edge forward
   while {($nextID!=-1) && ($nextID!=[lindex $edge 0])} {
      set i [lindex $edges $edgeIndex($nextID)]
      set nextID [lindex $i 7]
      redrawEdge $i $color $direction [expr {($width==0) ? [lindex $i 10]: $width}] $compID
   }
   
   if {$nextID==-1} {
      # redraw from the given edge backwards
      while {$prevID!=-1} {
      	 set i [lindex $edges $edgeIndex($prevID)]
     	 set prevID [lindex $i 6]
      	 redrawEdge $i $color $direction [expr {($width==0) ? [lindex $i 10]: $width}] $compID
      }
   }
   
#  printData
}
#**************************************************
proc redrawArc {edge color direction width compID} {
   global edges crossings edgeIndex

   # if width is 0, then all edges will be redrawn with 
   # their orignal width. If width is nonzero, then
   # all edges will be redrawn with the new given width
   
   # this procedure redraws an ARC (not a closed component!)
   # whose first or last edge is the given edge, with
   # the given color, direction and compID
   
   set sixOrSeven [expr {([lindex $edge 6]==-1) ? 7 : 6}]
   set ID [lindex $edge 0]
   while {$ID!=-1} {
      set i [lindex $edges $edgeIndex($ID)]
      redrawEdge $i $color $direction [expr {($width==0) ? [lindex $i 10]: $width}] $compID
      set ID [lindex $i $sixOrSeven]
   }
}
#**************************************************
proc reverseSelection { } {
   global edges 
   
   # make a list of selected edges with at most one edge per component
   
   set components {}
   foreach edge $edges {
      if {([lindex $edge 10]==4) && ([lsearch $components [lindex $edge 8]]==-1)} {
      	 lappend components [lindex $edge 8]
      	 lappend edgesToReverse $edge
      }
   }
   
   if {[info exists edgesToReverse]} {
      foreach edge $edgesToReverse {
      	 redrawComponent $edge [lindex $edge 5] opposite  1 [lindex $edge 8]
      }
      unset edgesToReverse
   }
   
   unset components
   findGaussCode
#  printData
}
#**************************************************
proc reverseAll { } {
   global edges
   
   set components {}
   foreach edge $edges {
      if {[lsearch $components [lindex $edge 8]]==-1} {
      	 lappend components [lindex $edge 8]
      	 lappend edgesToReverse $edge
      }
   }
   
   foreach edge $edgesToReverse {
     redrawComponent $edge [lindex $edge 5] opposite  1 [lindex $edge 8]
   }
   unset components
   if {[info exists edgesToReverse]} {
      unset edgesToReverse
   }
   findGaussCode
#  printData
}
#*******************************************#
#    crossing manipulation procedures       #
#*******************************************#
proc findCrossings { } {
   global edges crossings crossingID
   
   # finds all crossings between the last edge in the list of edges
   # and all previous edges, and adds these crossings to the list 
   # of crossings.
   # Draws the new crossings.

   set newEdge [lindex $edges [expr [llength $edges]-1]]
   set a2 [lindex $newEdge 1]
   set b2 [lindex $newEdge 2]
   set c2 [lindex $newEdge 3]
   set d2 [lindex $newEdge 4]
   foreach oldEdge $edges {
      set a1 [lindex $oldEdge 1]
      set b1 [lindex $oldEdge 2]
      set c1 [lindex $oldEdge 3]
      set d1 [lindex $oldEdge 4]
      set det [expr ($c1-$a1)*($b2-$d2)-($a2-$c2)*($d1-$b1)]
      if {$det!=0} {
      	 set sign [expr -$det/abs($det)]
      	 set t [expr double(($b2-$d2)*($a2-$a1)+($c2-$a2)*($b2-$b1))/double($det)]
	 set s [expr double(($b1-$d1)*($a2-$a1)+($c1-$a1)*($b2-$b1))/double($det)]
	 if {$t<1 && $t>0 && $s<1 && $s>0} {
	    set crossX [expr round((1-$t)*$a1+$t*$c1)]
	    set crossY [expr round((1-$t)*$b1+$t*$d1)]
	    
	    set lengthOldEdge [expr sqrt(($c1-$a1)*($c1-$a1)+($d1-$b1)*($d1-$b1))]
	    set oldCrossStartX [expr round($crossX-5*($c1-$a1)/$lengthOldEdge)]
	    set oldCrossStartY [expr round($crossY-5*($d1-$b1)/$lengthOldEdge)]
	    set oldCrossEndX [expr round($crossX+5*($c1-$a1)/$lengthOldEdge)]
	    set oldCrossEndY [expr round($crossY+5*($d1-$b1)/$lengthOldEdge)]
	    set oldCrossingEdge\
	     	[list $oldCrossStartX $oldCrossStartY $oldCrossEndX $oldCrossEndY]
	    
	    set lengthNewEdge [expr sqrt(($c2-$a2)*($c2-$a2)+($d2-$b2)*($d2-$b2))]
	    set newCrossStartX [expr round($crossX-5*($c2-$a2)/$lengthNewEdge)]
	    set newCrossStartY [expr round($crossY-5*($d2-$b2)/$lengthNewEdge)]
	    set newCrossEndX [expr round($crossX+5*($c2-$a2)/$lengthNewEdge)]
	    set newCrossEndY [expr round($crossY+5*($d2-$b2)/$lengthNewEdge)]
	    set newCrossingEdge\
	        [list $newCrossStartX $newCrossStartY $newCrossEndX $newCrossEndY]
	    	
	    set diskID [.linksmith.c create oval [expr $crossX-4] [expr $crossY-4] [expr $crossX+4] [expr $crossY+4] \
	       -fill white -outline white] 
	    set crossingArcID [.linksmith.c create line [lindex $newCrossingEdge 0] [lindex $newCrossingEdge 1]\
	    	[lindex $newCrossingEdge 2] [lindex $newCrossingEdge 3]\
	    	 -fill [lindex $newEdge 5] ]
	    lappend crossings [list $crossingID  $crossX $crossY\
	        [lindex $oldEdge 0] $t [lindex $oldEdge 5]\
	        $oldCrossingEdge\
	        [lindex $newEdge 0] $s [lindex $newEdge 5]\
	        $newCrossingEdge secondOnTop $crossingArcID $diskID $sign]
	    incr crossingID
	 }
      }
   }
#  printData
}
#******************************************
proc changeCrossing {crossing} {
   global edges crossings gaussCode  edgeIndex

   # delete the crossing arc
   .linksmith.c delete [lindex $crossing 12]
   
   # redraw the crossing
   set index [lsearch $crossings $crossing]
   if {[lindex $crossing 11] == "firstOnTop"} {
      set crossingEdge [lindex $crossing 10]
      set width [lindex [lindex $edges $edgeIndex([lindex $crossing 7])]  10]
      set crossingEdgeID [.linksmith.c create line [lindex $crossingEdge 0] [lindex $crossingEdge 1]\
      	 [lindex $crossingEdge 2] [lindex $crossingEdge 3]\
      	 -fill [lindex $crossing 9] -width $width]
      set crossing [lreplace $crossing 11 11 "secondOnTop"]
   } else {
      set crossingEdge [lindex $crossing 6]
      set width [lindex [lindex $edges $edgeIndex([lindex $crossing 3])]   10]
      set crossingEdgeID [.linksmith.c create line [lindex $crossingEdge 0] [lindex $crossingEdge 1]\
      	 [lindex $crossingEdge 2] [lindex $crossingEdge 3]\
      	 -fill [lindex $crossing 5] -width $width]
      set crossing [lreplace $crossing 11 11 "firstOnTop"]
      
   }   
   set crossing [lreplace $crossing 12 12 $crossingEdgeID]
   set crossing [lreplace $crossing 14 14 [expr -1*[lindex $crossing 14]]]
   set crossings [lreplace $crossings $index $index $crossing]
   
   # recompute Gauss code
   if {[llength $gaussCode]>0} {
      set crossID [lindex $crossing 0]
      set found 0
      foreach comp $gaussCode {
      	 set index [lsearch $comp $crossID]
      	 if {$index!=-1} {
      	    set index1 $index
      	    set index2 [lsearch $gaussCode $comp]
      	    incr found
      	    if {$found==2} break
      	 }
      	 set index [lsearch $comp [expr -$crossID]]
      	 if {$index!=-1} {
      	    set index3 $index
      	    set index4 [lsearch $gaussCode $comp]
      	    incr found
      	    if {$found==2} break
      	 }
      }
      set gaussCode [lreplace $gaussCode $index2 $index2\
      	 [lreplace [lindex $gaussCode $index2] $index1 $index1 [expr -$crossID]]]
      set gaussCode [lreplace $gaussCode $index4 $index4\
      	 [lreplace [lindex $gaussCode $index4] $index3 $index3 $crossID]]      
   }
#  printData   
}
    
#*******************************************#
#          misc procedures                  #
#*******************************************#
proc initGlobalVariables { } {
   global colors nextColor arcInProgress vertexDragInProgress\
      	 edges crossings edgeIndex\
      	 gaussCode DTcode\
      	 crossingID nextComponentID segmentID
      	 
   set colors {navy red orange1  MediumAquamarine\
               magenta  PaleGreen  MediumBlue\
      	       LawnGreen  thistle  DeepSkyBlue LightSlateGray}
   set nextColor [lindex $colors 0]
   set arcInProgress 0
   set vertexDragInProgress 0
   set edges {}
   if {[info exists edgeIndex]} {unset edgeIndex}
   set crossings {}
   set gaussCode {}
   set DTcode {}
   set crossingID 1  
   set nextComponentID 1
   set segmentID 1
}
#******************************************
proc advanceColor { } {
   global colors nextColor
   
   set n [lsearch $colors $nextColor]
   set nextColor [expr {$n<[llength $colors]-1 ?\
   	 [lindex $colors [expr $n+1]] : [lindex $colors 0]}]
}
#******************************************
proc dist {a b c d} {

   expr sqrt(($c-$a)*($c-$a)+($d-$b)*($d-$b))   
}
#******************************************
proc generalPosition {a b c d} {
   global crossings edges
   
   # returns false if edge {a b c d} is too short
   # or it its interior cuts through a crossing
   # or another vertex
   
   set length [dist $a $b $c $d]
   if {$length<25} {
      return 0
   }
   foreach i $crossings {
      set x [lindex $i 1]
      set y [lindex $i 2]
      if {[dist $x $y $a $b]+[dist $x $y $c $d]<[expr $length + 1]} {
      	 return 0
      }
   }
   foreach i $edges {
      set x [lindex $i 1]
      set y [lindex $i 2]
      if {(($x==$a) && ($y==$b)) || (($x==$c) && ($y==$d))} {
      } else {
      	 if {[dist $x $y $a $b]+[dist $x $y $c $d]<[expr $length + 1]} {
      	    return 0
      	 }
      }
      set x [lindex $i 3]
      set y [lindex $i 4]
      if {(($x==$a) && ($y==$b)) || (($x==$c) && ($y==$d))} {
      } else {
      	 if {[dist $x $y $a $b]+[dist $x $y $c $d]<[expr $length + 1]} {
            return 0
      	 }
      }
   }

   return 1
}
#******************************************
proc arcsTest { } {
   global edges
   
   # returns 1 if there is a valence 1 vertex, 0 otherwise
   foreach i $edges {
      if {([lindex $i 6]==-1) || ([lindex $i 7]==-1)} {
      	 return 1   
      }
   }
   return 0
}
#******************************************
proc eraseCanvas { } {
   global edges crossings rubberEdgeID
      
   foreach edge $edges {
      .linksmith.c delete [lindex $edge 9]
   }
   if {[info exists rubberEdgeID]} {
      .linksmith.c delete $rubberEdgeID
   }
   foreach crossing $crossings {
      .linksmith.c delete [lindex $crossing 12]
      .linksmith.c delete [lindex $crossing 13]
   }
   initGlobalVariables
}
#******************************************
proc startOver { } {
   global currentFilename
   
   eraseCanvas
   if {$currentFilename!="untitled"} {
      writeDataToFile $currentFilename
   }
#  printData
}
#***********************************************#
#  Gauss, DT, & Millett encoding procedures     #
#***********************************************#
proc findGaussCode { } {
   global edges crossings gaussCode 
   
   if {[arcsTest]} {
      set gaussCode {}
   } else {
      set edgeDistCross {}
      foreach i $crossings {
      	 if {[lindex $i 11]=="firstOnTop"} {
      	    lappend edgeDistCross [list [lindex $i 3] [lindex $i 4] [lindex $i 0]]
      	    lappend edgeDistCross [list [lindex $i 7] [lindex $i 8] [expr -[lindex $i 0]]]
      	 } else {
      	    lappend edgeDistCross [list [lindex $i 3] [lindex $i 4] [expr -[lindex $i 0]]]
      	    lappend edgeDistCross [list [lindex $i 7] [lindex $i 8] [lindex $i 0]]
      	 }
      }
      foreach edge $edges {
      	 lappend edgeDistCross [lindex $edge 0]
      }
      set edgeDistCross [lsort $edgeDistCross]
      
      if {[array size crossPerEdge]>0} {
      	 unset crossPerEdge
      }
      foreach i $edgeDistCross {
      	 if {[info exists crossPerEdge([lindex $i 0])]} {
      	    set crossPerEdge([lindex $i 0]) [concat $crossPerEdge([lindex $i 0]) [lindex $i 2]]
      	 } else {
      	    set crossPerEdge([lindex $i 0]) {}
      	 }
      }
   
      if {[array size edgePairsPerComp]>0} {
      	 unset edgePairsPerComp
      }
      foreach edge $edges {
      	 lappend edgePairsPerComp([lindex $edge 8]) [list  [lindex $edge 0] [lindex $edge 7]]
      }
      set gaussCode {}
      foreach compID [array names edgePairsPerComp] {
      	 if {[array size pairs]>0} {unset pairs}
      	 foreach i $edgePairsPerComp($compID) {
      	    set pairs([lindex $i 0]) [lindex $i 1]
      	 }
      	 set edgeCycle  [lindex $edgePairsPerComp($compID) 0]
      	 set start [lindex $edgeCycle 0] 
      	 set edgeID [lindex $edgeCycle 1]
      	 set component $crossPerEdge($start)
      	 set component [concat $component $crossPerEdge($edgeID)]
      	 while {$edgeID!=$start} {
      	    set edgeID $pairs($edgeID)
      	    if {$edgeID!=$start} {
      	       lappend edgeCycle $edgeID
      	       set component [concat $component $crossPerEdge($edgeID)]
      	    }
      	 }
      	 
      	 lappend gaussCode $component
      }
#     printData
   }
}
#**************************************************#
proc findDTcode { } {
   global gaussCode DTcode 
   
   set DTcode {}
   # if diagram is of a knot only, find DT code
   if {[llength $gaussCode]==1} {
      set comp [lindex $gaussCode 0]
      set twiceNoOfCrossings [llength $comp]
      for {set j 0} {$j<$twiceNoOfCrossings} {incr j 2} {
      	 set cross [lindex $comp $j]
      	 set match [expr -$cross]
      	 lappend DTcode [expr ($cross/abs($cross))*(1+[lsearch $comp $match])]
      }
#  printData
   }
}
#*********************************************************#
proc findMillettCode { } {
   global crossings gaussCode millettExportID tmp_dir
   
   if {$gaussCode!={}} {
      foreach crossing $crossings {
      	 set millett([lindex $crossing 0],sign)\
	    [expr {([lindex $crossing 14]==1) ? "+" : "-" }]
      }
      foreach component $gaussCode {
      	 for {set i 0} {$i<[llength $component]} {incr i 1} {
	    set crossing [lindex $component $i]
	    set absCrossing [expr abs($crossing)]
	    set nextCrossing [expr {($i==[expr [llength $component]-1]) ? \
	       [lindex $component 0]:[lindex $component [expr $i+1]]}]
	    set absNextCrossing [expr abs($nextCrossing)]
	    if {$nextCrossing>0} {
	       set arriveLetter c
	    } else {
	       set arriveLetter [expr {($millett($absNextCrossing,sign)== "+") ? "b" : "d" }]
	    }
	    if {$crossing>0} {
	       set departLetter a
	    } else {
	       set departLetter [expr {($millett($absCrossing,sign)=="+") ? "d" : "b" }]
	    }
	    set millett($absCrossing,$departLetter) $absNextCrossing$arriveLetter
	    set millett($absNextCrossing,$arriveLetter) $absCrossing$departLetter
      	 }
      }
      
      set f [open $tmp_dir/K[pid]millett.out w]
      puts $f [format "%1d.%7d:" [llength $crossings] $millettExportID]
      incr millettExportID
      foreach crossing $crossings {
      	 set i [lindex $crossing 0]
      	 puts -nonewline $f $i
         puts -nonewline $f $millett($i,sign)
         puts -nonewline $f $millett($i,a)
         puts -nonewline $f $millett($i,b)
         puts -nonewline $f $millett($i,c)
         puts            $f $millett($i,d)
      }
      
      close $f
      
      unset millett
   }
}
#*******************************************#
#  edge selection and deletion procedures   #
#*******************************************#
proc selectEdge {edge } {
   
   set width [expr {([lindex $edge 10]==1)? 4:1}]
   redrawEdge $edge [lindex $edge 5] same $width [lindex $edge 8]
#  printData
}
#******************************************
proc selectComponent {edge } {
   global edges 
   
   set width [expr {([lindex $edge 10]==4) ? 1: 4}]
   redrawComponent $edge [lindex $edge 5] same $width [lindex $edge 8]
 }
#******************************************
proc deleteEdge {edgeID compRenumbering} {
   global edges crossings crossingID nextComponentID edgeIndex
 
   # erase the edge
   set edge [lindex $edges $edgeIndex($edgeID)]   
   .linksmith.c delete [lindex $edge 9]
   
   # find edge in the list of edges and delete it
   set edges [lreplace $edges $edgeIndex($edgeID) $edgeIndex($edgeID)]
   
   # rebuild edgeIndex array
   unset edgeIndex
   for {set i 0} {$i <[llength $edges]} {incr i} {
      set edgeIndex([lindex [lindex $edges $i] 0]) $i
   }
    
   # find the other edges that were connected to this
   # one and change their prev and next values to -1
   if {[lindex $edge 6]!=-1} {
      set prevEdge [lindex $edges $edgeIndex([lindex $edge 6])]
      set prevEdge [lreplace $prevEdge 7 7 -1]
      set edges [lreplace $edges $edgeIndex([lindex $edge 6]) $edgeIndex([lindex $edge 6]) $prevEdge]
   }
   set nextID [lindex $edge 7]
   if {$nextID!=-1} {
      set nextEdge [lindex $edges $edgeIndex($nextID)]
      set nextEdge [lreplace $nextEdge 6 6 -1]
      set edges [lreplace $edges $edgeIndex($nextID) $edgeIndex($nextID) $nextEdge]
   }

   # Removing this edge may have split an arc into two arcs.
   # If so, we need to give one of the arcs a new compID
   # UNLESS we are dragging a vertex, in which case it is
   # convenient to leave the two arcs with the same compID
   # because when the drag ends we will immediately rejoin them
   if {$compRenumbering == "yes"} {
      while {$nextID!=-1} {
      	 set nextEdge [lindex $edges $edgeIndex($nextID)]
      	 set nextEdge [lreplace $nextEdge 8 8 $nextComponentID]
      	 set edges [lreplace $edges $edgeIndex($nextID) $edgeIndex($nextID) $nextEdge]
      	 set nextID [lindex $nextEdge 7]
      }
      incr nextComponentID
   }

   # find all of the crossings that were related to 
   # this edge and delete them
   foreach crossing $crossings {
      if {([lindex $crossing 3]==$edgeID) || ([lindex $crossing 7]==$edgeID)} {
      	 .linksmith.c delete [lindex $crossing 12]
      	 .linksmith.c delete [lindex $crossing 13]
      	 set i [lsearch $crossings $crossing]
      	 set crossings [lreplace $crossings $i $i] 
      }
   }
   
   # renumber the crossings with consecutive integers
   # starting from 1
   for {set i 0} {$i<[llength $crossings]} {incr i} {
      set crossings [lreplace $crossings $i $i [lreplace [lindex $crossings $i] 0 0 [expr $i +1]]]
   }
   
   # set crossingID to the next unused integer
   set crossingID [expr $i +1]
#  printData  
}
#******************************************
proc deleteselectedEdges { compRenumbering } {
   global  edges
   
   foreach edge $edges {
      if {[lindex $edge 10]==4} {
      	 deleteEdge [lindex $edge 0] $compRenumbering
      }
   }
   findGaussCode
#  printData
}
#*******************************************#
#       vertex dragging procedures          #
#*******************************************#
proc initiateDragVertex { } {
   global edges nearbyVertexInfo vertexDragInProgress\
      lastX lastY lastX2 lastY2  color rubberEdgeID rubberEdgeID2\
      originalX originalY compID end dragVertexValence\
      prevEdgeID prevEdgeID2 edgeIndex
   
   .linksmith.c config -cursor circle
   set dragVertexValence [lindex $nearbyVertexInfo 3]
   set originalX [lindex $nearbyVertexInfo 0]
   set originalY [lindex $nearbyVertexInfo 1]
   set edge1 [lindex $nearbyVertexInfo 2]
   set color [lindex $edge1 5]
   set compID [lindex $edge1 8]
   set end [lindex $nearbyVertexInfo 4]
   if {$end=="start"} {
      set lastX [lindex $edge1 3]
      set lastY [lindex $edge1 4]
      set prevEdgeID [lindex $edge1 7]
      if {$dragVertexValence==2} {
      	 set edge2 [lindex $edges $edgeIndex([lindex $edge1 6])]
      	 set lastX2 [lindex $edge2 1]
      	 set lastY2 [lindex $edge2 2]
      	 set rubberEdgeID2 [.linksmith.c create line\
      	     $lastX2 $lastY2 $originalX $originalY -fill $color]
      	 set prevEdgeID2 [lindex $edge2 6]
      	 if {$prevEdgeID2!=-1} {
      	    set prevEdge2 [lindex $edges $edgeIndex($prevEdgeID2)]
      	 }
      }
   } else {
      # end is finish
      set lastX [lindex $edge1 1]
      set lastY [lindex $edge1 2]
      set prevEdgeID [lindex $edge1 6]
      if {$dragVertexValence==2} {
      	 set edge2 [lindex $edges $edgeIndex([lindex $edge1 7])]
      	 set lastX2 [lindex $edge2 3]
      	 set lastY2 [lindex $edge2 4]
     	 set rubberEdgeID2 [.linksmith.c create line\
      	     $lastX2 $lastY2 $originalX $originalY -fill $color]
       	 set prevEdgeID2 [lindex $edge2 7]
      	 if {$prevEdgeID2!=-1} {
      	    set prevEdge2 [lindex $edges $edgeIndex($prevEdgeID2)]
      	 }
      }
   }
   if {$prevEdgeID!=-1} {
      set prevEdge [lindex $edges $edgeIndex($prevEdgeID)]
   }
   # delete the edges that are connected to the drag vertex
   # but set component renumbering to "no"
   # in this case we do not want to renumber the components
   # since we will be immediately rejoining them when the drag ends
   deleteEdge [lindex $edge1 0] "no"
   if {$dragVertexValence==2} {
      deleteEdge [lindex $edge2 0] "no"
   }
   set rubberEdgeID [.linksmith.c create line\
       $lastX $lastY $originalX $originalY -fill $color]
   set vertexDragInProgress 1
}
#******************************************
proc endDragVertex {x y} {
   global vertexDragInProgress lastX lastY lastX2 lastY2\
      rubberEdgeID rubberEdgeID2 edges\
      dragVertexValence nearbyVertexInfo\
      prevEdgeID prevEdgeID2 end compID color originalX originalY\
      segmentID edgeIndex
   
   set vertexDragInProgress 0
   .linksmith.c config -cursor top_left_arrow 
   .linksmith.c delete $rubberEdgeID
   set nearbyVertexValence [nearVertex 5 $x $y]
   if {$nearbyVertexValence==1} {
      set nearX [lindex $nearbyVertexInfo 0]
      set nearY [lindex $nearbyVertexInfo 1]
   }
   if {$dragVertexValence==1} {
      if {($nearbyVertexValence==1) && ([generalPosition $nearX $nearY $lastX $lastY])} {
      	 #connect to another arc end
      	 set otherEdge [lindex $nearbyVertexInfo 2]
      	 set otherEdgeID [lindex $otherEdge 0] 
      	 if {[lindex $otherEdge 8]==$compID} {
      	    # connect two ends of same component,
      	    # or connect to what was formerly a different arc,
      	    # but which was just redrawn in same color, direction 
      	    # and compID prior to calling endDragVertex
      	    # a second time
      	    if {$end=="finish"} {
      	       set tag [.linksmith.c create line $lastX $lastY $nearX $nearY\
      	          -fill $color -arrow last]
      	       set edge [list $segmentID $lastX $lastY $nearX $nearY $color $prevEdgeID $otherEdgeID $compID $tag 1]
      	       if {$prevEdgeID!=-1} {
      	          set index $edgeIndex($prevEdgeID)
      	          set prevEdge [lindex $edges $index]
      	          set edges [lreplace $edges $index $index [lreplace $prevEdge 7 7 $segmentID]]
      	       }
      	       set index $edgeIndex([lindex $otherEdge 0])
      	       set edges [lreplace $edges $index $index [lreplace $otherEdge 6 6 $segmentID]]
      	       lappend edges $edge
      	       set edgeIndex([lindex $edge 0]) [expr [llength $edges]-1]
      	       incr segmentID
      	       findCrossings
      	    } else {
      	       set tag [.linksmith.c create line  $nearX $nearY $lastX $lastY\
      	          -fill $color -arrow last]
      	       set edge [list $segmentID $nearX $nearY $lastX $lastY $color $otherEdgeID $prevEdgeID $compID $tag 1]
      	       if {$prevEdgeID!=-1} {
      	          set index $edgeIndex($prevEdgeID)
      	          set prevEdge [lindex $edges $index]
      	          set edges [lreplace $edges $index $index [lreplace $prevEdge 6 6 $segmentID]]
      	       }
      	       set index $edgeIndex([lindex $otherEdge 0])
      	       set edges [lreplace $edges $index $index [lreplace $otherEdge 7 7 $segmentID]]
      	       lappend edges $edge
      	       set edgeIndex([lindex $edge 0]) [expr [llength $edges]-1]
      	       incr segmentID
      	       findCrossings
      	    }
      	 } else {
      	    # connect two different arcs
      	    # redraw arc to which we are connecting, and then connect
      	    if {$end== "finish"} {
      	       if {[lindex $nearbyVertexInfo 4]== "start"} {
       	          redrawArc $otherEdge $color same 0 $compID
     	       } else {
       	          redrawArc $otherEdge $color opposite 0 $compID
      	       }
      	    } else {
      	       if {[lindex $nearbyVertexInfo 4]== "start"} {
       	          redrawArc $otherEdge $color opposite 0 $compID
      	       } else {
       	          redrawArc $otherEdge $color same 0 $compID
      	       }
      	    }
      	    endDragVertex $nearX $nearY
      	 }
      } else {
      	 # nearbyVertexValence was not 1 or proposed new edge was not in general position
      	 set nearbyEdge [nearEdge 1 $x $y]
      	 if { ($nearbyVertexValence==2) || ($nearbyEdge!={}) ||\
      	    (![generalPosition $x $y $lastX $lastY])} {
      	    # restore original edge
      	    set x $originalX
      	    set y $originalY
      	 } 
      	 # draw new edge to whitespace or restore original edge
      	 if {$end=="finish" } {
      	    set tag [.linksmith.c create line $lastX $lastY $x $y\
      	       -fill $color -arrow last]
      	    set edge [list $segmentID $lastX $lastY $x $y $color $prevEdgeID -1 $compID $tag 1]
      	       if {$prevEdgeID!=-1} {
      	          set index $edgeIndex($prevEdgeID)
      	          set prevEdge [lindex $edges $index]
      	          set edges [lreplace $edges $index $index [lreplace $prevEdge 7 7 $segmentID]]
      	       }
      	    lappend edges $edge
      	    set edgeIndex([lindex $edge 0]) [expr [llength $edges] -1]
      	    incr segmentID
      	    findCrossings
      	 } else {
      	    # $end is "start"
      	    set tag [.linksmith.c create line  $x $y $lastX $lastY\
      	       -fill $color -arrow last]
      	    set edge [list $segmentID $x $y $lastX $lastY $color -1 $prevEdgeID $compID $tag 1]
      	       if {$prevEdgeID!=-1} {
      	          set index $edgeIndex($prevEdgeID)
      	          set prevEdge [lindex $edges $index]
      	          set edges [lreplace $edges $index $index [lreplace $prevEdge 6 6 $segmentID]]
      	       }
     	    lappend edges $edge
      	    set edgeIndex([lindex $edge 0]) [expr [llength $edges] -1]
      	    incr segmentID
      	    findCrossings
      	 }
      }
   } else {
      # we were dragging a valence two vertex
      .linksmith.c delete $rubberEdgeID2
      # see if proposed new edges are in general position
      set nearbyEdge [nearEdge 1 $x $y]
      if {($nearbyVertexValence>0) || ($nearbyEdge!={}) ||\
      	  (![generalPosition $x $y $lastX $lastY]) || (![generalPosition $x $y $lastX2 $lastY2]) ||\
      	  ([expr [dist $lastX $lastY $lastX2 $lastY2] + [dist $lastX2 $lastY2 $x $y]-1]\
      	   <[dist $lastX $lastY $x $y])||\
      	   ([expr [dist $lastX $lastY $lastX2 $lastY2] + [dist $lastX $lastY $x $y]-1]\
      	   <[dist $lastX2 $lastY2 $x $y])}  {                                                  
      	 set x $originalX
      	 set y $originalY 
      }
      if {$end=="finish"} {
      	 set tag1 [.linksmith.c create line $lastX $lastY $x $y -fill $color -arrow last]
      	 set tag2 [.linksmith.c create line $x $y $lastX2 $lastY2 -fill $color -arrow last]
      	 set edge1 [list $segmentID $lastX $lastY $x $y $color $prevEdgeID [expr $segmentID +1] $compID $tag1 1]
      	 incr segmentID
      	 set edge2 [list $segmentID $x $y $lastX2 $lastY2 $color [expr $segmentID-1] $prevEdgeID2  $compID $tag2 1]
      	 if {$prevEdgeID!=-1} {
      	    set index $edgeIndex($prevEdgeID)
      	    set prevEdge [lindex $edges $index]
      	    set edges [lreplace $edges $index $index [lreplace $prevEdge 7 7 [expr $segmentID-1]]]
      	 }
      	 lappend edges $edge1
      	 set edgeIndex([lindex $edge1 0]) [expr [llength $edges] -1]
      	 findCrossings
      	 if {$prevEdgeID2!= -1} {
      	    set index $edgeIndex($prevEdgeID2)
      	    set prevEdge2 [lindex $edges $index]
      	    set edges [lreplace $edges $index $index [lreplace $prevEdge2 6 6 $segmentID]]
      	 }
      	 lappend edges $edge2
      	 set edgeIndex([lindex $edge2 0]) [expr [llength $edges] -1]
      	 incr segmentID
      	 findCrossings
      } else {
      	 # end is start
      	 set tag1 [.linksmith.c create line $x $y $lastX $lastY -fill $color -arrow last]
      	 set tag2 [.linksmith.c create line $lastX2 $lastY2 $x $y -fill $color -arrow last]
      	 set edge1 [list $segmentID $x $y $lastX $lastY $color [expr $segmentID +1] $prevEdgeID $compID $tag1 1]
      	 incr segmentID
      	 set edge2 [list $segmentID $lastX2 $lastY2 $x $y $color $prevEdgeID2 [expr $segmentID -1] $compID $tag2 1]
      	 if {$prevEdgeID!=-1} {
      	    set index $edgeIndex($prevEdgeID)
      	    set prevEdge [lindex $edges $index]
      	    set edges [lreplace $edges $index $index [lreplace $prevEdge 6 6 [expr $segmentID-1]]]
      	 }
      	 lappend edges $edge1
      	 set edgeIndex([lindex $edge1 0]) [expr [llength $edges] -1]
      	 findCrossings
      	 if {$prevEdgeID2!= -1} {
      	    set index $edgeIndex($prevEdgeID2)
      	    set prevEdge2 [lindex $edges $index]
      	    set edges [lreplace $edges $index $index [lreplace $prevEdge2 7 7 $segmentID]]
      	 }
      	 lappend edges $edge2
      	 set edgeIndex([lindex $edge2 0]) [expr [llength $edges] -1]
      	 incr segmentID
      	 findCrossings
      }  
      unset lastX2
      unset lastY2
   }
} 
#*******************************************#
#       global diagram operations           #
#*******************************************#
proc reflect { } {
   global crossings 
   
   foreach i $crossings {
      changeCrossing $i  
   }
}
#******************************************
proc alternate { } {
   global gaussCode singles
   
   
   # alternate will not apply to diagrams that include arcs
   # these can be recognized by having empty gaussCode
   
   if {[llength $gaussCode]>0} {
      set singles {}
      set done {}
      foreach comp $gaussCode {
      	 lappend done 0
      }
      # done is a list of flags, one per component
      # when all of the flags are 1, we have dealt
      # with every component
      # As each component is dealt with, we may encounter 
      # crossings between that comp and another comp
      # these crossings are recorded in the single list
      
      set notDone 1
      while {$notDone==1} {
      	 if {[llength $singles]>0} {
       	    set cross [expr -[lindex $singles 0]]
      	    foreach comp $gaussCode {
      	       if {[lsearch $comp $cross]!=-1} {
      	          set index [expr [lsearch $comp $cross]%2]
      	          if {($index==0)} {
      	             if {$cross>0} {
      	             	set height 1
      	             } else {
      	             	set height -1
      	             }
      	          } else {
      	             if {$cross>0} {
      	             	set height -1
      	             } else {
      	             	set height 1
      	             }
     	          }
     	          set index [lsearch $gaussCode $comp]
     	          set done [lreplace $done $index $index 1]
     	          alternateComponent $comp $height
     	          break
      	       }
      	    }
      	 } else {
      	    # singles is empty, ie there are no comps
      	    # that are connected to previously handled
      	    # comps that still need to be handled.
      	    # Hence, go to the first component yet to be
      	    # made alternating. (This will apply the first time
      	    # and again, only if the diagram is not connected.)
      	    
      	    set index [lsearch $done 0]
      	    set done [lreplace $done $index $index 1]
      	    set comp [lindex $gaussCode $index]
      	    set height 1
      	    alternateComponent $comp $height
      	 }

      	 # see if all the done flags are 1
      	 set notDone 0
      	 foreach flag $done {
      	    if {$flag==0} {
      	       set notDone 1
      	       break
      	    } 
      	 }
      }
#     printData
   }
}
#******************************************
proc alternateComponent {comp height} {
   global crossings  singles gaussCode

   # we are given a component (of the Gauss Code)
   # and an initial height of either 1 or -1
   # First go through the component and find out
   # which crossings need to be changed. The crossings
   # should alternate in sign, with the sign of the 
   # first one matching the initial height
   # collect all of the crossings to change into a list
   
   set index [lsearch $gaussCode $comp]
   
   set crossingsToChange {}
   foreach cross $comp {
      if {[expr $height*$cross]<0} {
      	 if {[lsearch $crossingsToChange [expr abs($cross)]]==-1} {
      	    lappend crossingsToChange [expr abs($cross)]
      	 }
      }
      set height [expr -$height]
   }
   
   # having found all the crossings to change, 
   # we will now change them.
   # calling changeCrossing will redo the guassCode
   
   foreach cross $crossingsToChange {
      foreach j $crossings {
      	 if {[lindex $j 0]==$cross} {
      	    changeCrossing $j
      	    break
      	 }
      }
   }
   
   # some crossings in this component may have been
   # crossings between this component and another component
   # These crossings are already set the way they should be
   # and they therefore determine how all the other crossings
   # on those other components should be.
   # So we will record these crossings in a list
   # called "singles" that can then be accessed
   # by the calling procedure, alternate.
   
   #first get the new gaussCode
   set comp [lindex $gaussCode $index]
   
   foreach cross $comp {
      if {[lsearch $singles [expr -$cross]]==-1} {
      	 lappend singles $cross
      } else {
      	 set index [lsearch $singles [expr -$cross]]
      	 set singles [lreplace $singles $index $index]
      }
   }
}
