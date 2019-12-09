temps de latence FIFO (buffers)
temps de latence callback

En vrai, ce qu'il faudrait faire : comparer entrée sortie réelle entre entrée micro et sortie haut parleur


Dans le rapport : 

Discuter le choix fait:
- Taille des buffers (trop petits : moins de données à traiter, mais plus de buffers ; trop grand : inverse)
- Nombre de buffers
- Taille de la réponse impulsionnelle : les derniers échantillons sont très petits, leur contribution est faible, on va donc très peu dégrader les performances si on ne les prend pas en compte.

Evaluation qualitative du résultat (pas de mesure type EQM car temps réel, sauf si lecture .wav)

Test à faire :

- Taille de buffer, tout égal par ailleurs
- Taille de la réponse impulsionnelle, tout égal par ailleurs.

fft
- fftr : r car entrée est un signal réel 
	- 3 paramètres :
		- 1er : sert à la fois d'entrée et de sortie. tableau de taille le nombre de point de la fft, pour la partie réelle de la fft (rappel nfft peut être supérieur à la taille du signal)
		- 2eme : tableau de la taille de la fft pour la partie imaginaire de la fft
		- 3eme : int nombre de points de la fft
- ifft : revenir dans le domaine temporel à partir de la partie réeelle et imaginaire de la fft
	- revenir dans l

