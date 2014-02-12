# Dialog chapter

proc fileselectResources {} {
    # path is used to enter the file name
    option add *Fileselect*path.relief		sunken	startup
    option add *Fileselect*path.background	white	startup
    option add *Fileselect*path.foreground	black	startup
    # Text for the label on pathname entry
    option add *Fileselect*l.text		File:	startup
    # Text for the OK and Cancel buttons
    option add *Fileselect*ok*text		OK	startup
    option add *Fileselect*ok*underline		0	startup
    option add *Fileselect*cancel.text		Cancel	startup
    option add *Fileselect*cancel.underline 	0	startup
    # Size of the listbox
    option add *Fileselect*list.width		20	startup
    option add *Fileselect*list.height		20	startup
    option add *Fileselect*font -adobe-helvetica-*-r-*-*-12-*-*-*-*-*-*-*
    # Colors
    option add *Fileselect*Background #a18fff
    option add *Fileselect*activeBackground #ff00ae
    option add *Fileselect*arrow*Background #816fff
    option add *Fileselect*entry*highlightThickness 2
    option add *Fileselect*entry*highlightColor #ff00ae
    option add *Fileselect*troughColor blue
    option add *Fileselect*activeForeground black
    option add *Fileselect*selectBackground #ff00ae
    option add *Fileselect*selectForeground black
}

# fileselectreboot returns the selected pathname, or {}
proc fileselect {{why "File Selection"} {default {}} {mustExist 1} } {
	global fileselect
	
	set fileselect(why) $why

	set t [toplevel .fileselect -bd 4 -class Fileselect -bg blue]

	wm geometry $t +250+250
	fileselectResources
    
	message $t.msg -aspect 1000 -text $why
	pack $t.msg -side top -fill x
  
	# Create a read-only entry for the current directory
	set fileselect(dirEnt) [entry $t.dir -width 15 \
		-relief flat -state disabled]
	pack $t.dir -side top -fill x
    
	# Create an entry for the pathname
	# The value is kept in fileselect(path)
	frame $t.top
	label $t.top.l -padx 0
	set e [entry $t.top.path \
		-textvariable fileselect(path)]
	pack $t.top -side top -fill x
	pack $t.top.l -side left
	pack $t.top.path -side right -fill x -expand true

    
	# Create a listbox to hold the directory contents
	set lb [listbox $t.list \
		-yscrollcommand [list $t.scroll set]]
	scrollbar $t.scroll -command [list $lb yview]

	# Create the OK and Cancel buttons
	# The OK button has a rim to indicate it is the default
	frame $t.fr1
	frame $t.fr1.buttons -bd 10
	frame $t.fr1.buttons.ok -bd 2 -relief sunken
	set ok [button $t.fr1.buttons.ok.b \
		-command fileselectOK]
	set can [button $t.fr1.buttons.cancel \
		-command fileselectCancel]
		
	set fileselect(overwriteFrame) [frame $t.overwrite]
	frame $t.overwrite.buttons
	message $t.overwrite.msg -text "OK to overwrite?" -width 100 -bg red\
	        -justify center	
	button $t.overwrite.buttons.yes -text Yes -bg green\
	   -command "set fileselect(overwrite) 1"	
	button $t.overwrite.buttons.no -text No -bg green\
	   -command "set fileselect(overwrite) 0"
	   			
	# Pack the list, scrollbar, and button box
	# in a horizontal stack below the upper widgets
	pack $t.list -side left -fill both -expand true
	pack $t.scroll -side left -fill y
	pack $t.fr1 $t.overwrite -side top -fill both -expand true
	pack $t.fr1.buttons -side left -fill both
	pack $t.fr1.buttons.ok $t.fr1.buttons.cancel \
		-side top -padx 10 -pady 5
	pack $t.fr1.buttons.ok.b -padx 4 -pady 4
	pack $t.overwrite.msg $t.overwrite.buttons -side top
	pack $t.overwrite.buttons.yes $t.overwrite.buttons.no -side left\
	        -padx 2 -pady 5
	
	pack forget $fileselect(overwriteFrame)
	
	fileselectBindings $t $e $lb $ok $can

	# Initialize variables and list the directory
	if {[string length $default] == 0} {
		set fileselect(path) {}
		set dir [pwd]
	} else {
                set strtmp [file tail $default]
		set fileselect(path) [string trim $strtmp]
		set dir [file dirname $default]
	}
	set fileselect(dir) {}
	set fileselect(done) 0
	set fileselect(mustExist) $mustExist

	# Wait for the listbox to be visible so
	# we can provide feedback during the listing 
	tkwait visibility .fileselect.list
	fileselectList $dir

	tkwait variable fileselect(done)
	destroy $t
	return $fileselect(path)
}
proc fileselectBindings { t e lb ok can } {
	# t - toplevel
	# e - name entry
	# lb - listbox
	# ok - OK button
	# can - Cancel button

	# Elimate the all binding tag because we
	# do our own focus management
	foreach w [list $e $lb $ok $can] {
	    bindtags $w [list $t [winfo class $w] $w]
	}
	# Dialog-global cancel binding
	bind $t <Control-c> fileselectCancel

	# Entry bindings
	bind $e <Return> fileselectOK
	bind $e <space> fileselectComplete

	# A single click, or <space>, puts the name in the entry
	# A double-click, or <Return>, selects the name
	bind $lb <space> "fileselectTake $%W ; focus $e"
	bind $lb <Button-1> \
		"fileselectClick %W %y ; focus $e"
	bind $lb <Return> "fileselectTake %W ; fileselectOK"
	bind $lb <Double-Button-1> \
		"fileselectClick %W %y ; fileselectOK"

	# Focus management.  	# <Return> or <space> selects the name.
	bind $e <Tab> "focus $lb ; $lb select set 0"
	bind $lb <Tab> "focus $e"

	# Button focus.  Extract the underlined letter
	# from the button label to use as the focus key.
	foreach but [list $ok $can] {
		set char [string tolower [string index  \
			[$but cget -text] [$but cget -underline]]]
		bind $t <Alt-$char> "focus $but ; break"
	}
	bind $ok <Tab> "focus $can"
	bind $can <Tab> "focus $ok"

	# Set up for type in
	focus $e
}

proc fileselectList { dir {files {}} } {
	global fileselect

	# Update the directory display
	set e $fileselect(dirEnt)
	$e config -state normal
	$e delete 0 end
	# get just one "/" at beginning of string
	set dir1 [string trimleft $dir "/"]
	append dir2 / $dir1
	$e insert 0 $dir2
	$e config -state disabled
	# scroll to view the tail end
	$e xview moveto 1

	.fileselect.list delete 0 end
	
	#trim rightmost slash, except when it's the only character
	set fileselect(dir) [string trimright $dir /]
	if {[string length $fileselect(dir)] == 0} {set fileselect(dir) "/"}
	if ![file isdirectory $dir] {
	    .fileselect.list insert 0 "Bad Directory"
	    return
	}
	.fileselect.list insert 0 Listing...
	update idletasks
	.fileselect.list delete 0
	if {[string length $files] == 0} {
		# List the directory and add an
		# entry for the parent directory
		set files [glob -nocomplain $fileselect(dir)/*]
		.fileselect.list insert end ../
	}
	# Sort the directories to the front
	set dirs {}
	set others {}
	foreach f [lsort $files] {
		if [file isdirectory $f] {
			lappend dirs [file tail $f]/
		} else {
			lappend others [file tail $f]
		}
	}
	foreach f [concat $dirs $others] {
		.fileselect.list insert end $f
	}
}
proc fileselectOK {} {
	global fileselect

	# Handle the parent directory specially
	
	#Deal with case where current directory is root
	
	#if {[string length $fileselect(dir)] == 1 &&\
	#    [string compare $fileselect(path) "../"] == 0} {
	#   set fileselect(path) {}
	#   fileselectList "/"
	#   return
	#}
	
	if {[regsub {^\.\./?} $fileselect(path) {} newpath] != 0} {
		set fileselect(path) $newpath
		set fileselect(dir) [file dirname $fileselect(dir)]
		#set fileselect(dir) [file dirname $fileselect(dir)]
		fileselectOK
		return
	}
    
	set path $fileselect(dir)/$fileselect(path)
    
	if [file isdirectory $path] {
		set fileselect(path) {}
		fileselectList $path
		return
	}
	
	# In Save As mode, verify that it's OK to overwrite existing file
	
	if {[string compare $fileselect(why) "Save As ..."] == 0} {
	   if {[file exists $path]} {
	      pack $fileselect(overwriteFrame) -side top -fill both -expand true
	      tkwait variable fileselect(overwrite)
	      if {$fileselect(overwrite) == 0} {
	         set fileselect(done) 1
	         set fileselect(path) {}
	         return
	      }
	   } else {
	      exec touch $path
	   }
	}
	
	if [file exists $path] {
		set fileselect(path) $path
		set fileselect(done) 1
		return
	}
	# Neither a file or a directory.
	# See if glob will find something
	if [catch {glob $path} files] {
		# No, perhaps the user typed a new
		# absolute pathname
		if [catch {glob $fileselect(path)} path] {
			# Nothing good
			if {$fileselect(mustExist)} {
				# Attempt completion
				fileselectComplete
			} elseif [file isdirectory \
				[file dirname $fileselect(path)]] {
				# Allow new name
				set fileselect(done) 1
			}
			return
		} else {
			# OK - try again
			set fileselect(dir) [file dirname $fileselect(path)]
			set fileselect(path) [file tail $fileselect(path)]
			fileselectOK
			return
		}
	} else {
		# Ok - current directory is ok,
		# either select the file or list them.
		if {[llength [split $files]] == 1} {
			set fileselect(path) $files
			fileselectOK
		} else {
			set fileselect(dir) [file dirname [lindex $files 0]]
			fileselectList $fileselect(dir) $files
		}
	}
}
proc fileselectCancel {} {
	global fileselect
	set fileselect(done) 1
	set fileselect(path) {}
}

proc fileselectClick { lb y } {
	# Take the item the user clicked on
	global fileselect
	set fileselect(path) [$lb get [$lb nearest $y]]
}
proc fileselectTake { lb } {
	# Take the currently selected list item
	global fileselect
	set fileselect(path) [$lb get [$lb curselection]]
}

proc fileselectComplete {} {
	global fileselect

	# Do file name completion
	# Nuke the space that triggered this call
	set fileselect(path) [string trim $fileselect(path) \t\ ]

	# Figure out what directory we are looking at
	# dir is the directory
	# tail is the partial name
	if {[string match /* $fileselect(path)]} {
		set dir [file dirname $fileselect(path)]
		set tail [file tail $fileselect(path)]
	} elseif [string match ~* $fileselect(path)] {
		if [catch {file dirname $fileselect(path)} dir] {
			return	;# Bad user
		}
		set tail [file tail $fileselect(path)]
	} else {
		set path $fileselect(dir)/$fileselect(path)
		set dir [file dirname $path]
		set tail [file tail $path]
	}
	# See what files are there
	set files [glob -nocomplain $dir/$tail*]
	if {[llength [split $files]] == 1} {
		# Matched a single file
		set fileselect(dir) $dir
		set fileselect(path) [file tail $files]
	} else {
		if {[llength [split $files]] > 1} {
			# Find the longest common prefix
			set l [expr [string length $tail]-1]
			set miss 0
			# Remember that files has absolute paths
			set file1 [file tail [lindex $files 0]]
			while {!$miss} {
				incr l
				if {$l == [string length $file1]} {
					# file1 is a prefix of all others
					break
				}
				set new [string range $file1 0 $l]
				foreach f $files {
					if ![string match $new* [file tail $f]] {
						set miss 1
						incr l -1
						break
					}
				}
			}
			set fileselect(path) [string range $file1 0 $l]
		}
		fileselectList $dir $files
	}
}
