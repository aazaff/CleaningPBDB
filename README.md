**Notice This Repository is Now Deprecated**
These functions are now part of the [velociraptr](https://github.com/paleobiodb/paleobiodb_utilities/tree/master/velociraptr) package.

# quantitativeFossils.R
R Functions for downloading, cleaning, culling, or analyzing fossil data from the Paleobiology Database. Developed and maintained by [Andrew Zaffos](https://macrostrat.org) as part of the [Paleobiology Database](https://paleobiodb.org) and [Macrostrat Database](https://macrostrat.org) tech development initiatives at the University of Wisconsin - Madison.

## Contents
+ [Creative Commons License](#creative-commons-license) # The creative commons license for modules in this repository.
+ [Version and Change Log](#version-and-change-log) # Information about changes to this repository.
+ [communityMatrix.R](#communitymatrixr) # Downloading and cleaning [Paleobiology Database](https://paleobiodb.org) (PBDB) data, and making a community matrix.
+ [cullMatrix.R](#cullmatrixr) # Culling a communty matrix of depauperate samples and rare taxa.
+ [subsampleRichness.R](#subsamplerichnessr) # A set of subsampling functions for standardizing sampled taxonomic richness.
+ [partitionDiversity.R](#partitiondiversityr) # A set of functions for calculating alpha, beta, and gamma diversity of a community matrix.

## Creative Commons License
All code within the [paleobiologyDatabase.R](https://github.com/aazaff/paleobiologyDatabase.R) repository is covered under a Creative Commons License [(CC BY-NC 4.0)](http://creativecommons.org/licenses/by-nc/4.0/). This license requires attribution to [Andrew A. Zaffos](https://macrostrat.org/#contact) and [Steven M. Holland](https://strata.uga.edu), and does not allow for commercial use.

## Version and Change Log
This is v0.03 of the paleobiologyDatabase.R repository. The repository has four functional modules: [communityMatrix.R](#communitymatrixr), [cullMatrix.R](#cullmatrixr), [subsampleRichness.R](#subsamplerichnessr), and [partitionDiversity.R](#partitiondiversityr). Two incomplete modules are also currently uploaded: basicStatistics.R and gaussianOccupancy.R. These modules are still under development and their use is discouraged.

The next module will add support for several dual-concept diversity indices (e.g., True Shannon's Entropy) to some of these modules.

+ v0.037 - Changed the package names to quantitativeFossils
+ v0.036 - Changed the ````ageRanges( )```` function so that it no longer rounds ages to the nearest million years.
+ v0.035 - Optimized the ````presenceMatrix( )```` and ````abundanceMatrix( )```` functions. It now produces an identical output at 30x faster. The arguments have been changed to make it clearer what makes up the rows and columns of the matrix.
+ v0.034 - Upgraded the ````cleanGenus( )```` function to [cleanRank( )](#cleanrank-), so that it will clean any taxonomic field - e.g., family, order - in addition to genus.
+ v.0.033 - Added [downloadPaleogeography( )](#downloadpaleogeography-) function. Downloads a map of paleocontinent orientation as a shapefile. Accepts an age between 541 and 0 mys.
+ v.0.032 - Added [ageRanges( )](#ageranges-) function, which finds the age range of each taxon. Changed [abundanceMatrix( )](#abundancematrix-) and [presenceMatrix( )](#presencematrix-) to accept taxon ranks other than genus.
+ v.0.031 - Fixed a bug with the error messages for [resampleIndividuals( )](#resampleindividuals-) and [subsampleIndividuals( )](#subsampleindividuals-)
+ v.0.030 - Added [partitionDiversity.R](#partitiondiversityr) module.
+ v.0.025 - Upgraded [downloadPBDB( )](#downloadpbdb-) to use https instead of http.
+ v.0.024 - Removed support for [basicStatistics.R](#basicstatisticsr) module until additional functions come online.
+ v.0.023 - Removed  communityMatrix( ) and replaced it with the identical [presenceMatrix( )](#presencematrix-) function to make it more explicit that it is creating a presence-absence dataset. Added the [abundanceMatrix( )](#abundancematrix-) function, which makes a matrix with abundances.
+ v.0.022 - Added [basicStatistics.R](#basicstatisticsr) module. Currently only has one supported function, [mestimateMean( )](#mestimatemean-), which calculates the least inverse squares M-estimated mean and error. More functions coming soon.
+ v.0.021 - Added [resampleIndividuals( )](#resampleindividuals-) to [subsampleRichness.R](#subsamplerichnessr) module.
+ v.0.020 - Added [subsampleRichness.R](#subsamplerichnessr) module. Changed repository name from CleaningPBDB to paleobiologyDatabase.R. Added new function, [softCull( )](#softcull-), to cullMatrix.R module.
+ v.0.010 - Added [communityMatrix.R](#communitymatrixr) and [cullMatrix.R](#cullmatrixr) modules.

## communityMatrix.R
A set of functions for downloading data from the Paleobiology Database, and organizing it into a community matrix of samples by taxa, where "samples" are based on  one of the variables in the Paleobiology Database - e.g., Plate ID, Geologic Age.

Can be accessed directly in R using:

````R
source("https://raw.githubusercontent.com/aazaff/paleobiologyDatabase.R/master/communityMatrix.R")
````

##### downloadPBDB( )
````R
# Download data from PBDB by taxonomic group and geologic interval.

# Argument Taxa must be a vector of one or more taxon names (as a character string), no default.
# Argument StartInterval must be a single interval name accepted by the PBDB, default is "Pliocene"
# Argument StopInterval must be a single interval name accepted by the PBDB, default is "Pleistocene" 

DataPBDB<-downloadPBDB(Taxa=c("Bivalvia","Gastropoda"),StartInterval="Cambrian",StopInterval="Pleistocene")
````

##### downloadTime( )
````R
# Download Timescale definitions from Macrostrat.

# Argument Timescale must be a timescale recognized by the macrostrat API, no default
# A list of Timescale defs can be seen here https://macrostrat.org/api/defs/timescales?all

Epochs<-downloadTime(Timescale="international epochs")
````

##### downloadPaleogeography( )
````R
# Download a map of paleocontinents for a specific age from Macrostrat as a shapefile.
# Note that this makes use of the rgdal package and its dependencies.

# Argument Age is a numerical value ranging from 541 to 0 mys ago.

PaleoMap<-downloadPaleogeography(Age=0)
````

##### constrainAges( )
````R
# Assign fossil occurrences to different ages, then remove occurrences that are not temporally 
# constrained to a single interval.

# Argument DataPBDB is a dataset downloaded from the PBDB - i.e., using downloadPBDB( )
# Argument Timescale is a dataset downloaded from Macrostrat - i.e., using downloadTime( )

ConstrainedAges<-constrainAges(DataPBDB=DataPBDB,Timescale=Epochs)
````

##### ageRanges( )
````R
# Find the age range (min and max age) based on occurrence data in the PBDB for
# A particular level of the taxonomic hierarchy (e.g., genus, family, order)

# Argument DataPBDB is a dataset downloaded from the PBDB - i.e., using downloadPBDB( )
# Argument Taxonomy is a level of the taxonomic hierarchy - e.g., "genus"

AgeRanges<-ageRanges(DataPBDB=DataPBDB,Taxonomy="genus")
````

##### cleanRank( )
````R
# Cleans the a taxonomic rank field of the PBDB data by removing NA's. It also removes subgenera from the genus rank. 
# This is an upgraded version of the now deprecated cleanGenus( ) function.

# Argument DataPBDB is a dataset downloaded from the PBDB - i.e., using downloadPBDB( )
# Argument Rank is a taxonomic rank - i.e., "genus".

CleanedPBDB<-cleanRank(DataPBDB,Rank="genus")
````

##### presenceMatrix( )
````R
# Create a community matrix of samples v. species, using elements within one of the PBDB columns
# (e.g., geoplate, early_interval) as the definition of a sample. This creates a presence-absence
# matrix of 1's (presence) and 0's (absence).

# This is just a renamed version of the now deprecated function communityMatrix( ).

# Argument DataPBDB is a dataset downloaded from the PBDB - i.e., using downloadPBDB( )
# Argument Rows is the column name defining samples
# Argument Columns is the column name defining taxa

CommunityMatrix<-presenceMatrix(DataPBDB,Rows="geoplate",Columns="genus")
````

##### abundanceMatrix( )
````R
# Create a community matrix of samples v. species, using elements within one of the PBDB columns
# (e.g., geoplate, early_interval) as the definition of a sample. This creates an "abundance"
# matrix, which uses the number of occurrences a genus has within the "sample" as its abundance.
# Because the theoretical and operational meaning of occurrences in the Paleobiology Database is ill-defined
# I recommend using presenceMatrix( ) instead if possible.

# Argument DataPBBDB is a dataset downloaded from the PBDB - i.e., using downloadPBDB( )
# Argument Rows is the column name defining samples
# Argument Columns is the column name defining taxa

CommunityMatrix<-abundanceMatrix(DataPBDB,Rows="geoplate",Columns="genus")
````

## cullMatrix.R
A set of functions for removing depauperate and rare taxa from community matrices of samples by taxa.

Can be accessed directly in R using:

````R
source("https://raw.githubusercontent.com/aazaff/paleobiologyDatabase.R/master/cullMatrix.R")
````

##### cullMatrix( )
````R
# Cull a community matrix of depauperate samples and rare taxa. Written by S.M. Holland.

# Argument x is a community matrix, no default.
# Argument minOccurrences is the minimum number of occurrences for each taxon, default = 2
# Argument minDiversity is the minimum number of taxa within each sample, default=2

CulledMatrix<-cullMatrix(CommunityMatrix,minOccurrences=5,minDiversity=5)
````

##### softCull( )
````R
# A variant of cullMatrix( ) that returns NA when there are no samples or taxa left
# rather than throwing an error. Useful when culling multiple matrices in a loop, 
# and you would rather skip depauperate matrices than break the loop. 
# Not recommended otherwise.

# Argument x is a community matrix, no default.
# Argument minOccurrences is the minimum number of occurrences for each taxon, default = 2
# Argument minDiversity is the minimum number of taxa within each sample, default=2

CulledMatrix<-softCull(CommunityMatrix,minOccurrences=5,minDiversity=5)
````

## subsampleRichness.R
Functions for standardizing taxonomic richness. The multicore versions use the [doMC](https://cran.r-project.org/web/packages/doMC/vignettes/gettingstartedMC.pdf) package and its dependencies. This package is currently not available for windows.

Can be accessed directly in R using:

````R
source("https://raw.githubusercontent.com/aazaff/paleobiologyDatabase.R/master/subsampleRichness.R")
````

##### subsampleEvenness( )
````R
# A function that subsamples richness based on evenness. Often referred to as "Shareholder Quorum Subsampling".
# An optimized version of John Alroy's original function by Steven M. Holland.

# Argument Abundance is a vector of abundances.
# Argument Quota is a value between 0 and 1, the default is set to 0.9.
# Argument Trials determines how many iterations of the bootstrap are performed, default = 100
# Argument IgnoreSingletons determines whether or not to ignore singletons, default is FALSE.
# Argument ExcludeDominant determines whether or not to ignore the most dominant taxon
# Excluding the abundant taxon is recommended by Alroy, but the default is set to FALSE.

SubsampledRichness<-subsampleEvenness(Abundance,Quota=0.5,Trials=100,IgnoreSingletons=FALSE,ExcludeDominant=FALSE)
````

##### multicoreEvenness( )
````R
# A multicore version of subsampleEvenness( ). Be warned that multicoreEvenness( ) is not automatically
# faster than subsampleEvenness( ), particularly for a low number of trials. Its use is not recommended
# for small abundance vectors or a small numbers of trials. Requires the doMC package.

# Argument Abundance is a vector of abundances.
# Argument Quota is a value between 0 and 1, the default is set to 0.9.
# Argument Trials determines how many iterations of the bootstrap are performed, default = 1000
# Argument IgnoreSingletons determines whether or not to ignore singletons, default is FALSE.
# Argument ExcludeDominant determines whether or not to ignore the most dominant taxon
# Excluding the dominant taxon is recommended by Alroy, but the default is set to FALSE.
# Argument Cores sets the number of processor cores, default = 4.

SubsampledRichness<-multicoreEvenness(Abundance,Quota=0.5,Trials=100,IgnoreSingletons=FALSE,ExcludeDominant=FALSE,Cores=4)
````

##### subsampleIndividuals( )
````R
# A function that subsamples richness based on a fixed number of individuals. Often referred to as "rarefaction".

# Argument Abundance is a vector of abundances.
# Argument Quota is the number of individuals to be subsampled. If the Quota is greater than
# the number of individuals, the function will print a warning and return the unstandardized richness.
# Argument Trials determines how many iterations of the bootstrap are performed, default = 100

SubsampledRichness<-subsampleIndividuals(Abundance,Quota,Trials=100)
````

##### multicoreIndividuals( )
````R
# A multicore version of subsampleIndividuals( ). Be warned that multicoreIndividuals( ) is not automatically
# faster than subsampleIndividuals( ), particularly for a low number of trials. Its use is not recommended
# for small abundance vectors or a small numbers of trials. Requires the doMC package.

# Argument Abundance is a vector of abundances.
# Argument Quota is the number of individuals to be subsampled. If the Quota is greater than
# the number of individuals, the function will print a warning and return the unstandardized richness.
# Argument Trials determines how many iterations of the bootstrap are performed, default = 1000
# Argument Cores sets the number of processor cores, default = 4.

SubsampledRichness<-multicoreIndividuals(Abundance,Quota,Trials=1000,Cores=4)
````

##### resampleIndividuals( )
````R
# A specialized variant of subsampleIndividuals( ). If the quota is greater than the number of individuals
# it will switch to sampling with replacement. This allows for diversity in those samples to be lower
# than the quota. 

# Caution: This is a non-standard approach.

# Argument Abundance is a vector of abundances.
# Argument Quota is the number of individuals to be subsampled. 
# Argument Trials determines how many iterations of the bootstrap are performed, default = 100

ResampledIndividuals<-resampleIndividuals(Abundance,Quota,Trials=100)
````

## partitionDiversity.R
Functions for calculating alpha, beta, and gamma richness of a community matrix. See [communityMatrix.R](#communitymatrixr) for functions to make such a matrix with Paleobiology Database data and [cullMatrix.R](#cullmatrixr) for functions to cull and prepare
such a dataset. 

Some of these functions were presented in Holland, SM (2010) Additive diversity partitioning in palaeobiology: revisiting Sepkoski’s question. *Paleontology* 53:1237-1254. Namely, [taxonAlphaContributions( )](#taxonalphacontributions-), [taxonBetaContributions( )](#taxonbetacontributions-), and [sampleBetaContributions( )](#sampleBetaContributions-).

Other methods of beta calculation come from the equations presented in Tuomisto, H (2010) A diversity of beta diversities: straightening up a concept gone awry. Part 1. Defining beta diversity as a function of alpha and gamma diversity. *Ecography* 33:2-22.

Can be accessed directly in R using:

````R
source("https://raw.githubusercontent.com/aazaff/paleobiologyDatabase.R/master/partitionDiversity.R")
````

##### taxonAlphaContributions( )
````R
# Returns vector of each taxon’s contribution to alpha diversity. Written by S.M. Holland.

# Argument x is a community matrix of presence-absence data.

TaxonAlpha<-taxonAlphaContributions(x=PresenceMatrix)
````

##### taxonBetaContributions( )
````R
# Returns vector of each taxon’s contribution to beta diversity. 
# Be warned that if you are using a hierarchichal partitioning scheme 
# that this function *always* calculates between-sample beta,
# with sample being defined by your matrix. You must pre-aggregate samples 
# in the community matrix before you can calculate the beta 
# diversity of a higher level in the hierarchy. Written by S.M. Holland.

# Argument x is a community matrix of presence-absence data.

TaxonBeta<-taxonBetaContributions(x=PresenceMatrix)
````

##### sampleBetaContributions( )
````R
# Returns vector of each sample’s contribution to beta diversity. Written by S.M. Holland.

# Argument x is a community matrix of presence-absence data.

TaxonBeta<-sampleBetaContributions(x=PresenceMatrix)
````

##### meanAlpha( )
````R
# Returns mean alpha diversity (richness) of samples. Written by S.M. Holland.

# Argument x is a community matrix of presence-absence data.

AlphaDiversity<-meanAlpha(x=PresenceMatrix)
````

##### beta( )
````R
# Returns beta diversity (richness) of samples. Written by S.M. Holland.

# Argument x is a community matrix of presence-absence data.

BetaDiversity<-beta(x=PresenceMatrix)
````

##### gamma( )
````R
# Returns gamma (total) diversity (richness) of matrix. Written by S.M. Holland.

# Argument x is a community matrix of presence-absence data.

GammaDiversity<-gamma(x=PresenceMatrix)
````

##### traditionalAlpha( )
````R
# Calculate alpha diversity in the traditional manner, averaging sample richness.
# Should always be equal to meanAlpha( ) function.

# Argument x is a community matrix of presence-absence data.

AlphaDiversity<-traditionalAlpha(x=PresenceMatrix)
````

##### traditionalBeta( )
````R
# Calculate beta diversity in the traditional manner ADP manner Beta = Gamma - Alpha
# Should always be equal to beta( ) function.

# Argument x is a community matrix of presence-absence data.

BetaDiversity<-traditionalBeta(x=PresenceMatrix)
````

##### multiplicativeBeta( )
````R
# Calculate beta diversity in the traditional multiplicative manner. Beta = Gamma/Alpha

# Argument x is a community matrix of presence-absence data.

BetaDiversity<-multiplicativeBeta(x=PresenceMatrix)
````

##### completeTurnovers( )
````R
# Calculate Whittaker's effective species turnover, the number of complete effective species 
# turnovers among samples in the dataset. 
# Beta = (Gamma-Alpha)/Alpha

# Argument x is a community matrix of presence-absence data.

BetaDiversity<-completeTurnovers(x=PresenceMatrix)
````

##### proportionNonendemic( )
````R
# Proportional effective species turnover, the proporition of species in the region 
# not limited to a single sample - i.e., the 
# proportion of "non-endemic" taxa. Beta = (Gamma-Alpha)/Gamma

# Argument x is a community matrix of presence-absence data.

BetaDiversity<-proportionNonendemic(x=PresenceMatrix)
````
