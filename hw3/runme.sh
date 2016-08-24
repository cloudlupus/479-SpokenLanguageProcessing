#!/bin/bash
#David Shagam

for mode in eval decode; do
	for hmm in hmm1 hmm2; do
		for seq in seqs1 seqs2; do
   			./prog3 -$mode $seq.txt $hmm/{pi,A,B}.txt > ${mode}_${hmm}_${seq}.out
		done
	done
done

for hmm in hmm1 hmm2; do
	./prog3 -sample 20 $hmm/{pi,A,B}.txt > ${hmm}_samples.out 2> ${hmm}_states.out
	for i in $(seq 2 1000); do
		./prog3 -sample 20 $hmm/{pi,A,B}.txt >> ${hmm}_samples.out 2>> ${hmm}_states.out
	done
done