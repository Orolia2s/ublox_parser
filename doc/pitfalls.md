# Vue systeme

## ANT-OM : ANTena Operation Management

Pour la fonction : `ANT-OM` c'est à dire le module qui vérifie le bon fonctionnement de l'antenne GNSS utilisée dans l'application:
 1. Problème de Celestica qui a un splitter qui ne permet pas de détecter l'état de l'alimentation de l'antenne
 2. Problème où l'antenne est en bypass sur la carte (antenna_status = 3) mais journalctl nous affiche que le recepteur à un Fix3D. Le recepteur fonctionne mais ocillartord nous indique que l'antenne est en court-circuit et reste en holdover.
 3. Le recepteur peut fonctionner alors que l'antenne_status indique que l'antenne n'est pas connecté(antenna_status : 4).
 4. Ne pas diminuer trop la puissance du signal en en chainant les splitters, le recepteur ne detectera plus le signal.
 5. Un signal en sortie de simulateur qui n'est pas bien synchronisé peut généré un Fix3D sur recepteur mais peut empêcher oscillatord d'entrer dans ses boucles d'asservissement.
