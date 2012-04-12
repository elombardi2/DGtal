/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file freemanChainDisplay.cpp
 * @ingroup Examples
 * @author Bertrand Kerautret (\c kerautre@loria.fr )
 * LORIA (CNRS, UMR 7503), University of Nancy, France
 *
 * @date 2010/12/01
 * 
 * An example of FreemanChain display with background source image.
 *
 * This file is part of the DGtal library.
 */

//! [freemanChainFromImage-basicIncludes]
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "ConfigExamples.h"
//! [freemanChainFromImage-basicIncludes]

//! [freemanChainFromImage-ImageSetIncludes]
#include "DGtal/io/readers/PNMReader.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
//! [freemanChainFromImage-ImageSetIncludes]

//! [freemanChainFromImage-trackingFCIncludes]
#include "DGtal/topology/helpers/Surfaces.h"
#include "DGtal/geometry/curves/representation/FreemanChain.h"
//! [freemanChainFromImage-trackingFCIncludes]

//! [freemanChainFromImage-displayIncludes]
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "DGtal/io/Color.h"
//! [freemanChainFromImage-displayIncludes]

int main()
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD:examples/geometry/curves/representation/freemanChainFromImage.cpp
=======

  //! [freemanChainFromImage-imageImport]
  typedef DGtal::ImageContainerBySTLVector< Z2i::Domain, unsigned char> Image;
  std::string filename =  examplesPath + "samples/circleR10modif.pgm";
  Image image = DGtal::PNMReader<Image>::importPGM(filename); 
  //! [freemanChainFromImage-imageImport]
>>>>>>> 65df006... small change fior beug with image type <int> [work]...:examples/tutorial-examples/freemanChainFromImage.cpp
=======
=======
>>>>>>> 515ed21... update tutorial example using snippet.

  //! [freemanChainFromImage-imageImport]
  typedef DGtal::ImageContainerBySTLVector< Z2i::Domain, int> Image;
  std::string filename =  examplesPath + "samples/contourS.pgm";
  Image image = DGtal::PNMReader<Image>::importPGM(filename); 
  //! [freemanChainFromImage-imageImport]
<<<<<<< HEAD
>>>>>>> 515ed21... update tutorial example using snippet.
=======
>>>>>>> 515ed21... update tutorial example using snippet.
  
  //![freemanChainFromImage-ksspace]
  Z2i::KSpace ks;
  ks.init( image.domain().lowerBound(), image.domain().upperBound(), true );
  //![freemanChainFromImage-ksspace]

  //! [freemanChainFromImage-setAppend]
  Z2i::DigitalSet set2d (image.domain());
  SetPredicate<Z2i::DigitalSet> set2dPredicate( set2d );
<<<<<<< HEAD:examples/geometry/curves/representation/freemanChainFromImage.cpp
  SetFromImage<Z2i::DigitalSet>::append<Image>(set2d, image, 0, 255);
<<<<<<< HEAD
<<<<<<< HEAD
=======
  SetFromImage<Z2i::DigitalSet>::append<Image>(set2d, image, 1, 255);
=======
>>>>>>> 515ed21... update tutorial example using snippet.
=======
>>>>>>> 515ed21... update tutorial example using snippet.
  //! [freemanChainFromImage-setAppend]


  //! [freemanChainFromImage-displaySet]
  Board2D aBoard;
  aBoard << set2d;
  aBoard << image.domain();  
  //! [freemanChainFromImage-displaySet]

  //! [freemanChainFromImage-adj]
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 65df006... small change fior beug with image type <int> [work]...:examples/tutorial-examples/freemanChainFromImage.cpp
=======
>>>>>>> 515ed21... update tutorial example using snippet.
=======
>>>>>>> 515ed21... update tutorial example using snippet.
  SurfelAdjacency<2> sAdj( true );
  //! [freemanChainFromImage-adj]

  //! [freemanChainFromImage-extraction]
  std::vector< std::vector< Z2i::Point >  >  vectContoursBdryPointels;
  Surfaces<Z2i::KSpace>::extractAllPointContours4C( vectContoursBdryPointels,
                ks, set2dPredicate, sAdj );  
  //! [freemanChainFromImage-extraction]
  

  GradientColorMap<int> cmap_grad( 0, (const int)vectContoursBdryPointels.size() );
  cmap_grad.addColor( Color( 50, 50, 255 ) );
  cmap_grad.addColor( Color( 255, 0, 0 ) );
  cmap_grad.addColor( Color( 255, 255, 10 ) );
  cmap_grad.addColor( Color( 25, 255, 255 ) );
  cmap_grad.addColor( Color( 255, 25, 255 ) );
  cmap_grad.addColor( Color( 25, 25, 25 ) );
  
  //! [freemanChainFromImage-fcConstruction]
  for(unsigned int i=0; i<vectContoursBdryPointels.size(); i++){
    //  Constructing and displaying FreemanChains from contours. 
    FreemanChain<Z2i::Integer> fc (vectContoursBdryPointels.at(i));    
    //! [freemanChainFromImage-fcConstruction]
    //! [freemanChainFromImage-fcdysplay]
    aBoard << SetMode( fc.className(), "InterGrid" );
    aBoard<< CustomStyle( fc.className(), 
            new CustomColors(  cmap_grad(i),  Color::None ) );        
    aBoard << fc;
    //! [freemanChainFromImage-fcdysplay]
  }   

  aBoard.saveEPS("freemanChainFromImage.eps");
  return 0;
}
///////////////////////////////////////////////////////////////////////////////