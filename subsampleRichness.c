# Steve Holland's optimized SQS function
subsampleEvenness<-function(Abundance,Quota=0.9,Trials=100,IgnoreSingletons=FALSE,ExcludeDominant=FALSE) {
	Abundance<-Abundance[Abundance>0]
	if ((Quota <= 0 || Quota >= 1)) {
		stop('The SQS Quota must be greater than 0.0 and less than 1.0')
		}
 	# compute basic statistics
	Specimens<-sum(Abundance)
	NumTaxa<-length(Abundance)
	Singletons<-sum(Abundance==1)
	Doubletons<-sum(Abundance==2)
	Highest<-max(Abundance)
	MostFrequent<-which(Abundance==Highest)[1]
 	if (ExcludeDominant==FALSE) {
		Highest<-0
		MostFrequent<-0
		}
 	# compute Good's u
	U<-0
	if (ExcludeDominant==TRUE) {
		U<-1-Singletons/(Specimens-Highest)
		} 
	else {
		U<-1-Singletons/Specimens
		}
 	if (U==0) {
		stop('Coverage is zero because all taxa are singletons')
		}
 	# re-compute taxon frequencies for SQS
	FrequencyInitial<-Abundance-(Singletons+Doubletons/2)/NumTaxa
	Frequency<-FrequencyInitial/(Specimens-Highest)
 	# return if the quorum target is higher than estimated coverage
	if ((Quota>sum(Frequency)) || (Quota >= sum(Abundance))) {
		stop('SQS Quota is too large, relative to the estimated coverage')
		}
 	# create a vector, length equal to total number of specimens,
	# each value is the index of that species in the Abundance array
	IDS<-unlist(mapply(rep,1:NumTaxa,Abundance))
 	# subsampling trial loop
	Richness<-rep(0,Trials) # subsampled taxon richness
	for (Trial in 1:Trials) {
		Pool<-IDS # pool from which specimens will be sampled
		SpecimensRemaining<- length(Pool) # number of specimens remaining to be sampled
		Seen<-rep(0,NumTaxa) # keeps track of whether taxa have been sampled
		SubsampledFrequency<-rep(0,NumTaxa) # subsampled frequencies of the taxa
		Coverage<-0
 		while (Coverage<Quota) {
			# draw a specimen
			DrawnSpecimen<-sample(1:SpecimensRemaining,size=1)
			DrawnTaxon<-Pool[DrawnSpecimen]
 			# increment frequency for this taxon
			SubsampledFrequency[DrawnTaxon]<-SubsampledFrequency[DrawnTaxon]+1
 			# if taxon has not yet been found, increment the coverage
			if (Seen[DrawnTaxon]==0) {
				if (DrawnTaxon!=MostFrequent&&(IgnoreSingletons==0||Abundance[DrawnTaxon]>1)) {
					Coverage<-Coverage+Frequency[DrawnTaxon]
					}
				Seen[DrawnTaxon]<-1
 				# increment the richness if the Quota hasn't been exceeded,
				# and randomly throw back some draws that put the coverage over Quota
				if (Coverage<Quota || runif(1)<=Frequency[DrawnTaxon]) {
					Richness[Trial]<-Richness[Trial]+1
					} 
				else {
					SubsampledFrequency[DrawnTaxon]<-SubsampledFrequency[DrawnTaxon]-1
					}
				}
 			# decrease pool of specimens not yet drawn
			Pool[DrawnSpecimen]<-Pool[SpecimensRemaining]
			SpecimensRemaining<-SpecimensRemaining-1
			}
		}
 	# compute subsampled richness
	S2<-Richness[Richness>0]
	SubsampledRichness<-exp(mean(log(S2)))*length(S2)/length(Richness)
	return(round(SubsampledRichness,1))
	}
