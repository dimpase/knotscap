/*
 *  knotscape_hyp.c
 *
 *  Reads DT sequences and computes hyperbolic invariants.
 *
 *  1.   Compute assorted hyperbolic invariants:
 *          volume
 *          maximal cusp volume
 *          length of meridian, longitude and shortest curve on maximal cusp boundary
 *          symmetry group
 *
 *  2.   Compute centers and radii of horoballs, and coordinates of
 *          fundamental region
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SnapPea.h"

#define FALSE   0
#define TRUE    1

extern Triangulation    *DT_alpha_to_triangulation(char *aDTString);
extern Triangulation    *DT_int_to_triangulation(int aNumCrossings, int *aDTCode);
static char             describe_symmetry_action(IsometryList *aSymmetryList);
int                     compute_assorted_invariants(int option, char **argv);
int                     compute_horoballs();


FILE *fp, *gp1, *gp2;

int main(int argc, char **argv)
{
   int option;
   
   option = (int) (*argv[1] - '0');
   
   fp = fopen(argv[2], "r");
   if (option >  0) gp1 = fopen(argv[3], "w");
   if (option == 4) gp2 = fopen(argv[4], "w");   
 
   switch (option)
   {
      case 0: compute_assorted_invariants(option, argv);
      break;
      case 1: compute_assorted_invariants(option, argv);
      break;
      case 2: compute_assorted_invariants(option, argv);
      break;
      case 3: compute_assorted_invariants(option, argv);
      break;
      case 4: compute_horoballs();
   }
   
   return 0;
}



int compute_assorted_invariants(int option, char **argv)
{
    Triangulation   *theTriangulation;
    int             theNumCrossings,
                    theIndex,
                    *theIntDT,
                    i;

    if (option == 0)
    {
        char *path, *file_name;
        int word_length, j;
        
        path = malloc(100*sizeof(char));
        file_name = malloc(100*sizeof(char));
        
        path = argv[3];
        word_length=0; while (path[word_length] != 0) ++word_length;
        i=word_length;
        while (i >= 0 && path[i] != '/') --i;
        for (j=i+1; j<=word_length; ++j) file_name[j-i-1] = path[j];
        
        
        fscanf(fp, "%d%d", &theNumCrossings, &theIndex);
        theIntDT = (int *) malloc(theNumCrossings * sizeof(int));
        for (i = 0; i < theNumCrossings; i++) fscanf(fp, "%d", &theIntDT[i]);
        theTriangulation = DT_int_to_triangulation(theNumCrossings, theIntDT);
        free(theIntDT);
        if (get_complete_solution_type(theTriangulation) >=3)
	{
            gp1 = fopen(argv[3], "w");
	    fprintf(gp1, "Knot is apparently non-hyperbolic\n\n\n");
	    free_triangulation(theTriangulation);
	    return 0;
	}
        
        if (theTriangulation == NULL)
        {
            gp1 = fopen(argv[3], "w");
            fprintf(gp1, " ???\n");
            return 0;
        }
        
        set_triangulation_name(theTriangulation, file_name);
        save_triangulation(theTriangulation, argv[3]);
        
        free_triangulation(theTriangulation);
        
        return 0;
    }

    if (option > 0)
    while (fscanf(fp, "%d%d", &theNumCrossings, &theIndex) == 2)
    {
      
        fprintf(gp1, "%2d %6d  ", theNumCrossings, theIndex); 

        theIntDT = (int *) malloc(theNumCrossings * sizeof(int));
        for (i = 0; i < theNumCrossings; i++)
        {
            fscanf(fp, "%d", &theIntDT[i]);
            fprintf(gp1, "%4d", theIntDT[i]);
        }
	fprintf(gp1, "\n\n");
        
        theTriangulation = DT_int_to_triangulation(theNumCrossings, theIntDT);
        
        free(theIntDT);
	
	if (get_complete_solution_type(theTriangulation) >=3)
	{ 
	    fprintf(gp1, "Knot is apparently non-hyperbolic\n\n\n");
	    free_triangulation(theTriangulation);
	    continue;
	}
        
        if (theTriangulation == NULL)
        {
            fprintf(gp1, " ???\n");
            continue;
        }
 
        if (option == 1)
        {
            fprintf(gp1, "Volume:% 14.10f\n\n\n", volume(theTriangulation, NULL));
            continue;
        }
 
 
        if (option == 2)
        {
            SymmetryGroup   *theManifoldSymmetryGroup,
                            *theLinkSymmetryGroup;
            Triangulation   *unused1;
            Boolean         unused2;
            AbelianGroup    *theAbelianGroup;
            char            theSymType;
 
            theManifoldSymmetryGroup    = NULL;
            theLinkSymmetryGroup        = NULL;
            unused1                     = NULL;
 
            if (compute_symmetry_group( theTriangulation,
                                        &theManifoldSymmetryGroup,
                                        &theLinkSymmetryGroup,
                                        &unused1,
                                        &unused2) == func_OK)
            {
                if (symmetry_group_order(theManifoldSymmetryGroup)
                 != symmetry_group_order(theLinkSymmetryGroup))
                {
                    printf("extra symmetries in knot complement\n");
                    exit(0);
                }
 
                fprintf(gp1, "Symmetry type:   ");
                /*
                 *  Describe the knot's chirality and invertibility.
                 */
                theSymType = describe_symmetry_action(get_symmetry_list(theManifoldSymmetryGroup));
                
                switch (theSymType)
                {
                    case 'c':
                        fprintf(gp1, "chiral");
                        break;
                    case '+':
                        fprintf(gp1, "+amphicheiral");
                        break;
                    case '-':
                        fprintf(gp1, "-amphicheiral");
                        break;
                    case 'i':
                        fprintf(gp1, "reversible");
                        break;
                    case 'a':
                        fprintf(gp1, "fully amphicheiral");
                        break;
                    default:
                        fprintf(gp1, "bad symmetry group type\n");
 
                        exit(0);
                }
 
	        fprintf(gp1, "\n");
	
	        fprintf(gp1, "Symmetry group:  ");
	
	
	        /*
                 *  Riley has proved that the symmetry group of a
                 *  hyperbolic knot is cyclic or dihedral.
                 */
                if (symmetry_group_order(theManifoldSymmetryGroup) == 1)
                    fprintf(gp1, " 1  ");
                else if (symmetry_group_order(theManifoldSymmetryGroup) == 2)
                {
                    switch (theSymType)
                    {
                        case 'c':
                        case '+':
                            fprintf(gp1, "Z2 ");
                            break;
                        case '-':
                        case 'i':
                            fprintf(gp1, "D1 ");
                            break;
                        case 'a':
                        default:
                            fprintf(gp1, "bad symmetry group type\n");
                            exit(0);
                    }
                }
                else if (symmetry_group_is_dihedral(theManifoldSymmetryGroup) == TRUE)
                {
	          int sgo;  /* shorthand for symmetry group order */
	          sgo = symmetry_group_order(theManifoldSymmetryGroup);

	          if (sgo < 20)
	            fprintf(gp1, "D%d ", sgo / 2);
                  else
	            fprintf(gp1, "D%2d", sgo / 2);
	        }
	        else if (symmetry_group_is_abelian(theManifoldSymmetryGroup, &theAbelianGroup) == TRUE
                     && theAbelianGroup->num_torsion_coefficients == 1
                     && theAbelianGroup->torsion_coefficients[0] == symmetry_group_order(theManifoldSymmetryGroup))
                {
	          int sgo;
	          sgo = symmetry_group_order(theManifoldSymmetryGroup);
	          if (sgo < 10)
	            fprintf(gp1, "Z%d ", sgo);
                  else
	            fprintf(gp1, "Z%2d", sgo);
	        }
	        else
 
                {
                    fprintf(gp1, "bad symmetry group\n");
                    exit(0);
                }


                free_symmetry_group(theManifoldSymmetryGroup);
                free_symmetry_group(theLinkSymmetryGroup);
            }
            else
                fprintf(gp1, "This knot appears to be non-hyperbolic");
 
            fprintf(gp1, "\n\n");
            
            continue;
        }

 
        /*
         *  Invariants of the maximal cusp neighborhood
         */
 
        if (option == 3)
 
        {

           CuspNeighborhoods *theCuspNbhds;
           int j, k, xl1, xl2, yl1, yl2, numShorterCurves, coeff[100][2];
           double curveLength[100],
              stopping_displacement, mx, my, lx, ly,
              p1, p2, q1, q2, p11, p12, p21, p22, q11, q12, q21, q22, det,
              pwl, ml,
              theCuspVolume;
           Complex theMeridian, theLongitude;

           theCuspNbhds = initialize_cusp_neighborhoods(theTriangulation);

           stopping_displacement = get_cusp_neighborhood_stopping_displacement(theCuspNbhds, 0);
           set_cusp_neighborhood_displacement(theCuspNbhds, 0, stopping_displacement);

           get_cusp_neighborhood_translations(theCuspNbhds, 0, &theMeridian, &theLongitude);
           theCuspVolume = get_cusp_neighborhood_cusp_volume(theCuspNbhds, 0);


           /*
            *   introduce shorthand, and compute length of meridian
            */

           mx = theMeridian.real;
           my = theMeridian.imag;
           lx = theLongitude.real;
           ly = theLongitude.imag;
           ml = sqrt(mx*mx + my*my);

           fprintf(gp1, "Volume of maximal cusp neighborhood: % 14.10f\n", theCuspVolume);
 
           fprintf(gp1, "Longitudinal translation vector: (% 14.10f , % 14.10f )\n", lx, ly);
           fprintf(gp1, "Meridional translation vector:   (% 14.10f , % 14.10f )\n", fabs(mx), my);
 
           fprintf(gp1, "Length of longitude: % 14.10f\n", sqrt(lx*lx + ly*ly));
           fprintf(gp1, "Length of meridian:  % 14.10f\n", ml);
 
           numShorterCurves = 0;

           /*
            *   find all translates which fit inside the bounding box
            *   -ml < x < ml  ,  -ml < y < ml;
            *   solve system of two equations
            *
            */

           det = lx*my - mx*ly;     /* determinant of system */

           p11 = (1/det)*(my*(-ml) - mx*(-ml));
           q11 = (1/det)*(-ly*(-ml) + lx*(-ml));
           p12 = (1/det)*(my*(ml) - mx*(-ml));
           q12 = (1/det)*(-ly*(ml) + lx*(-ml));
           p21 = (1/det)*(my*(ml) - mx*(ml));
           q21 = (1/det)*(-ly*(ml) + lx*(ml));
           p22 = (1/det)*(my*(-ml) - mx*(ml));
           q22 = (1/det)*(-ly*(-ml) + lx*(ml));

           if (p11 < p12) p1 = p11; else p1 = p12;
           if (p21 < p1) p1 = p21;
           if (p22 < p1) p1 = p22;
           p1 = floor(p1);

           if (p11 > p12) p2 = p11; else p2 = p12;
           if (p21 > p2) p2 = p21;
           if (p22 > p2) p2 = p22;
           p2 = ceil(p2);

           if (q11 < q12) q1 = q11; else q1 = q12;
           if (q21 < q1) q1 = q21;
           if (q22 < q1) q1 = q22;
           q1 = floor(q1);

           if (q11 > q12) q2 = q11; else q2 = q12;
           if (q21 > q2) q2 = q21;
           if (q22 > q2) q2 = q22;
           q2 = ceil(q2);

           xl1 = 3*p1-1;
           xl2 = 3*p2+1;
           yl1 = 3*q1-1;
           yl2 = 3*q2+1;

           /*
            *   find curves not longer than 3 times meridian (if any)
            */

           for (j=xl1; j<=xl2; ++j) for (k=yl1; k<=yl2; ++k)
                 if ((pwl=sqrt((j*lx + k*mx)*(j*lx + k*mx) + (j*ly + k*my)*(j*ly + k*my)))
                       <= 3*ml + 1E-6  &&  j != 0  &&  k > 0)

                 {
                    int uj, uk;    /* check that j, k are relatively prime */
 
                    uj = abs(j);
                    uk = abs(k);
 
                    while (uk != 0)
                    {
                       int temp = uk;
                       uk = uj % uk;
                       uj = temp;
                    }
 
                    if (uj > 1) continue;   /* gcd is greater than 1 */
 
                    curveLength[numShorterCurves] = pwl;
                    coeff[numShorterCurves][0] = k;
                    coeff[numShorterCurves][1] = j;
                    ++numShorterCurves;
                 }
 
           if (numShorterCurves != 0)
           {
              fprintf(gp1, "Other short curves on boundary:\n");
              fprintf(gp1, "  M      L        length\n");
              for (i=0; i<=numShorterCurves - 1; ++i)
              {
                 fprintf(gp1, "%3d    %3d      ", coeff[i][0], coeff[i][1]);
                 fprintf(gp1, "% 14.10f", curveLength[i]);
                 if (curveLength[i] < ml) fprintf(gp1, "   ** shorter than meridian **");
                 fprintf(gp1, "\n");
              }
              fprintf(gp1, "\n");
           }
 
           fprintf(gp1, "\n\n");
 
           free_cusp_neighborhoods(theCuspNbhds);
        }

        free_triangulation(theTriangulation);

        verify_my_malloc_usage();
    }
    
    return 0;
}


static char describe_symmetry_action(
    IsometryList    *aSymmetryList)
{
    /*
     *  A symmetry may
     *
     *  (1)  preserve both the meridian and longitude,
     *  (2)  reverse  the meridian while preserving the longitude,
     *  (3)  preserve the meridian while reversing  the longitude, or
     *  (4)  reverse  both the meridian and longitude.
     *
     *  The identity symmetry is of type 1.
     *  If a symmetry group contains symmetries of two of the nontrivial
     *  types (i.e. types 2, 3 and 4), then it must contain the third
     *  nontrivial type as well (proof:  take the product).
     *  Therefore there are five types of symmetry groups, denoted by
     *  the following symbols:
     *
     *      'c'   contains type 1 symmetries only
     *              (chiral, noninvertible)
     *
     *      '+'   contains type 1 and 2 symmetries only
     *              (+amphichiral, noninvertible)
     *
     *      '-'   contains type 1 and 3 symmetries only
     *              (-amphichiral, noninvertible)
     *
     *      'i'   contains type 1 and 4 symmetries only
     *              (chiral, invertible)
     *
     *      'a'   contains type 1, 2, 3 and 4 symmetries
     *              (+-amphichiral, invertible)
     */

    Boolean hasType2,
            hasType3,
            hasType4;
    int     theNumIsometries,
            theCuspImage,
            theCuspMap[2][2],
            i;

    hasType2 = FALSE;
    hasType3 = FALSE;
    hasType4 = FALSE;

    theNumIsometries = isometry_list_size(aSymmetryList);

    for (i = 0; i < theNumIsometries; i++)
    {
        isometry_list_cusp_action(aSymmetryList, i, 0, &theCuspImage, theCuspMap);
        if (theCuspMap[0][0] == -1  &&  theCuspMap[1][1] == +1)
            hasType2 = TRUE;
        if (theCuspMap[0][0] == +1  &&  theCuspMap[1][1] == -1)
            hasType3 = TRUE;
        if (theCuspMap[0][0] == -1  &&  theCuspMap[1][1] == -1)
            hasType4 = TRUE;
    }

    if (!hasType2 && !hasType3 && !hasType4) return 'c';
    if ( hasType2 && !hasType3 && !hasType4) return '+';
    if (!hasType2 &&  hasType3 && !hasType4) return '-';
    if (!hasType2 && !hasType3 &&  hasType4) return 'i';
    if ( hasType2 &&  hasType3 &&  hasType4) return 'a';

    uFatalError("describe_symmetry_action", "compute_hyp_invariants");
    return ' '; /* keep the compiler happy */
}


int compute_horoballs()
{
    Triangulation   *theTriangulation;
    int             theNumCrossings,
                    theIndex, 
                    *theIntDT,
                    i;
    double          cut_off;


    while (fscanf(fp, "%3d%10d", &theNumCrossings, &theIndex) == 2)
    {
        /*
         *  Echo the input to the output.
         */
        /*fprintf(gp1, "%3d %8d\n", theNumCrossings, theIndex);*/

        /*
         *  Read the DT code.
         */
        theIntDT = (int *) malloc(theNumCrossings * sizeof(int));
        for (i = 0; i < theNumCrossings; i++)
        {
            fscanf(fp, "%3d", &theIntDT[i]);
        }

	/*
	 *  Read cut-off height for horoballs
	 */
	fscanf(fp, "%10lf", &cut_off);

        /*
         *  Decode theIntDT and triangulate the knot complement.
         */
        theTriangulation = DT_int_to_triangulation(theNumCrossings, theIntDT);

        /*
         *  Free the temporary array.
         */
        free(theIntDT);
	
	if (get_complete_solution_type(theTriangulation) >=3)
	{ 
	  free_triangulation(theTriangulation);
	  fprintf(gp1, "non-hyperbolic\n");
	  exit(0);
	}
	
	fprintf(gp1, "%3d %8d\n", theNumCrossings, theIndex);

        if (theTriangulation != NULL)
        {
	   CuspNeighborhoods *theCuspNbhds;
	   CuspNbhdHoroballList *theCuspNbhdHoroballList;
	   int numHoroballs, j, k, xl1, xl2, yl1, yl2;
	   double center_real[10001], center_imag[10001], radius[10001],
	      stopping_displacement, mx, my, lx, ly, u1, u2, v1, v2,
	      x0, y0, x1, y1, x2, y2, x3, y3, min_x, min_y, max_x, max_y,
	      p1, p2, q1, q2, p11, p12, p21, p22, q11, q12, q21, q22, det,
	      rad, cx, cy,
	      scale_x, scale_y, translate_x, translate_y;
	   Complex theMeridian, theLongitude;
	
	   theCuspNbhds = initialize_cusp_neighborhoods(theTriangulation);
	   
	   stopping_displacement = get_cusp_neighborhood_stopping_displacement(theCuspNbhds, 0);
	   set_cusp_neighborhood_displacement(theCuspNbhds, 0, stopping_displacement);
	   
           get_cusp_neighborhood_translations(theCuspNbhds, 0, &theMeridian, &theLongitude);
	   
	   
	   theCuspNbhdHoroballList = get_cusp_neighborhood_horoballs(theCuspNbhds, 0, 1, cut_off);
	   
	   numHoroballs = theCuspNbhdHoroballList -> num_horoballs;

	   for (i=0; i<=numHoroballs-1; ++i)
	   {
	      radius[i] = ((theCuspNbhdHoroballList -> horoball) + i) -> radius;
	      center_real[i] = ((theCuspNbhdHoroballList -> horoball) + i) -> center.real;
	      center_imag[i] = ((theCuspNbhdHoroballList -> horoball) + i) -> center.imag;
	   }
   
	   
	   /*
	    *  Scale so that fundamental region fits nicely in canvas
	    *  of dimensions 800x600.  First locate lowest leftmost
	    *  horoball of maximal radius.
	    */
	   
	   min_x = min_y = 1000.0;    /* impossibly large */
	   
	   for (i=0; i<=numHoroballs-1; ++i) if (radius[i] > .499999)
	   {
	      if (center_real[i] + center_imag[i] < min_x + min_y)
	      {
	        min_x = center_real[i];
		min_y = center_imag[i];
              }	      
	   }
	   
	   max_x = min_x + theMeridian.real + theLongitude.real;
	   max_y = min_y + theMeridian.imag + theLongitude.imag;
	   
	   if (theMeridian.real >= 0)
	      scale_x = 600/(max_x - min_x);
	   else
	      scale_x = 600/(max_x - min_x - 2*theMeridian.real);
	   translate_x  =  400  -  scale_x * (min_x + max_x) / 2;
	   scale_y = scale_x;
	   translate_y  =  300  -  scale_y * (min_y + max_y) / 2;
	   
	   for (i=0; i<=numHoroballs-1; ++i)
	   {
	      center_real[i] = center_real[i] * scale_x + translate_x;
	      center_imag[i] = center_imag[i] * scale_y + translate_y;
	      radius[i] = radius[i] * scale_x;
	   }
	   
	   theMeridian.real *= scale_x;
	   theMeridian.imag *= scale_x;
	   theLongitude.real *= scale_x;
	   theLongitude.imag *= scale_x;
	   
	   
	   min_x = min_x * scale_x + translate_x;
	   max_x = max_x * scale_x + translate_x;
	   min_y = min_y * scale_y + translate_y;
	   max_y = max_y * scale_y + translate_y;
	   
	   /*
	    *   print coordinates of vertices of fundamental region
	    */
	   
	   x0 = min_x;
	   y0 = min_y;
	   x1 = x0 + theMeridian.real;
	   y1 = y0 + theMeridian.imag;
	   x2 = max_x;
	   y2 = max_y;
	   x3 = x0 + theLongitude.real;
	   y3 = y0 + theLongitude.imag;
	   
	   fprintf(gp1, "% 9.4f % 9.4f\n", x0, y0);
	   fprintf(gp1, "% 9.4f % 9.4f\n", x1, y1);
	   fprintf(gp1, "% 9.4f % 9.4f\n", x2, y2);
	   fprintf(gp1, "% 9.4f % 9.4f\n", x3, y3);
	   	   
           /*
	    *   Write Postscript file header
	    */
	    
	   fprintf(gp2, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	   fprintf(gp2, "%%%%Title: Horoball Diagram of Knot %1d.%1d ;  Cut-off Diameter: %4.2f\n",
	                 theNumCrossings, theIndex, cut_off);
	   fprintf(gp2, "%%%%BoundingBox: 0 0 600 800\n");
	   fprintf(gp2, "%%%%Pages: 1\n");
	   fprintf(gp2, "%%%%DocumentData: Clean7Bit\n");
	   fprintf(gp2, "%%%%Orientation: Portrait\n");
	   fprintf(gp2, "%%%%EndComments\n");
	   fprintf(gp2, "%%%%Page: 1 1\n\n");
	   fprintf(gp2, "605 0 translate\n");
	   fprintf(gp2, "90 rotate\n\n");
	   fprintf(gp2, "50 562.5 moveto 750 562.5 lineto 750 37.5 lineto 50 37.5 lineto closepath\n");
	   fprintf(gp2, "clip newpath\n");
	   fprintf(gp2, "1 setlinewidth\n");
	   fprintf(gp2, "50 562.5 moveto 750 562.5 lineto 750 37.5 lineto 50 37.5 lineto closepath\n");
	   fprintf(gp2, "400 300 translate 7 8 div dup scale -400 -300 translate\n");
	   fprintf(gp2, ".3 setlinewidth\n");
	   fprintf(gp2, "% 9.4f % 9.4f  moveto\n", x0, y0);
	   fprintf(gp2, "% 9.4f % 9.4f  lineto\n", x1, y1);
	   fprintf(gp2, "% 9.4f % 9.4f  lineto\n", x2, y2);
	   fprintf(gp2, "% 9.4f % 9.4f  lineto\n", x3, y3);
	   fprintf(gp2, "closepath\n\n");

	   /*
	    *   find all translates which fit inside canvas, i.e.
	    *   whose centers satisfy  0 < x < 800 and 0 < y < 600
	    */
	   
	   mx = theMeridian.real;
	   my = theMeridian.imag;
	   lx = theLongitude.real;
	   ly = theLongitude.imag;
	   
	   /*
	    *   find coordinates of bounding box for original horoballs
	    */
	    
	   min_x = 1000.0;	   
	   for (i=0; i<=numHoroballs-1; ++i) if (center_real[i] - radius[i] < min_x)
	     min_x = center_real[i] - radius[i];
	   min_y = 1000.0;
	   for (i=0; i<=numHoroballs-1; ++i) if (center_imag[i] - radius[i] < min_y)
	     min_y = center_imag[i] - radius[i];
	   max_x = -1000.0;
	   for (i=0; i<=numHoroballs-1; ++i) if (center_real[i] + radius[i] > max_x)
	     max_x = center_real[i] + radius[i];
	   max_y = -1000.0;
	   for (i=0; i<=numHoroballs-1; ++i) if (center_imag[i] + radius[i] > max_y)
	     max_y = center_imag[i] + radius[i];
	   
	   /*
	    *   work out range of linear combinations needed to
	    *   fill canvas.  Solve system of two equations.
	    */
	   
	   det = lx*my - mx*ly;     /* determinant of system */
	   
	   p11 = (1/det)*(my*(-max_x) - mx*(-max_y));
	   q11 = (1/det)*(-ly*(-max_x) + lx*(-max_y));
	   p12 = (1/det)*(my*(800-min_x) - mx*(-max_y));
	   q12 = (1/det)*(-ly*(800-min_x) + lx*(-max_y));
	   p21 = (1/det)*(my*(800-min_x) - mx*(600-min_y));
	   q21 = (1/det)*(-ly*(800-min_x) + lx*(600-min_y));
	   p22 = (1/det)*(my*(-max_x) - mx*(600-min_y));
	   q22 = (1/det)*(-ly*(-max_x) + lx*(600-min_y));
	   
	   if (p11 < p12) p1 = p11; else p1 = p12;
	   if (p21 < p1) p1 = p21;
	   if (p22 < p1) p1 = p22;
	   p1 = floor(p1);
	
	   if (p11 > p12) p2 = p11; else p2 = p12;
	   if (p21 > p2) p2 = p21;
	   if (p22 > p2) p2 = p22;
	   p2 = ceil(p2);
	   
	   if (q11 < q12) q1 = q11; else q1 = q12;
	   if (q21 < q1) q1 = q21;
	   if (q22 < q1) q1 = q22;
	   q1 = floor(q1);
	
	   if (q11 > q12) q2 = q11; else q2 = q12;
	   if (q21 > q2) q2 = q21;
	   if (q22 > q2) q2 = q22;
	   q2 = ceil(q2);
	  
	   xl1 = (int) p1;
	   xl2 = (int) p2;
	   yl1 = (int) q1;
	   yl2 = (int) q2;
	   
	 
	   /*
	    *   print parameters needed by tk to draw circles
	    */

	   for (i=0; i<=numHoroballs-1; ++i)
	      for (j=xl1; j<=xl2; ++j) for (k=yl1; k<=yl2; ++k)
	         if (center_real[i] + radius[i] + j*lx + k*mx > -50 &&
		     center_real[i] - radius[i] + j*lx + k*mx <  850 &&
		     center_imag[i] + radius[i] + j*ly + k*my > -50 &&
		     center_imag[i] - radius[i] + j*ly + k*my <  650)   
		 {
		    u1 = center_real[i] + j*lx + k*mx - radius[i];
		    v1 = center_imag[i] + j*ly + k*my - radius[i];
		    u2 = center_real[i] + j*lx + k*mx + radius[i];
		    v2 = center_imag[i] + j*ly + k*my + radius[i];
		 		 
		    fprintf(gp1, "% 9.4f   ", u1);
		    fprintf(gp1, "% 9.4f   ", v1);
		    fprintf(gp1, "% 9.4f   ", u2);
		    fprintf(gp1, "% 9.4f   ", v2);
		    fprintf(gp1, "\n");
		    
                    rad = radius[i];
		    cx = u1 + rad;
		    cy = v1 + rad;
		    x0 = u2;
		    y0 = cy;
		    
		    fprintf(gp2, "% 9.4f  % 9.4f  moveto\n", x0, y0);
		    fprintf(gp2, "% 9.4f  % 9.4f  % 9.4f   0 360 arc\n", cx, cy, rad);
	         }
		    
	   
	   
	   /*
	    *   Write epilog of Postscript file
	    */
	   
	   fprintf(gp2, "stroke\n");
	   fprintf(gp2, "showpage\n");
	   fprintf(gp2, "%%%%EOF\n"); 
	   
	   
	   free_cusp_neighborhood_horoball_list(theCuspNbhdHoroballList);
	   free_cusp_neighborhoods(theCuspNbhds);
	

           free_triangulation(theTriangulation);
        }
        else
           printf(" ???\n");

        verify_my_malloc_usage();
    }
    
    return 0;
    
}
