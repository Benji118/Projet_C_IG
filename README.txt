README :

Options du makefile :
make all : generation de la bibliotheque et de l'ensemble des executables
make <name> : generation de la bibliotheque et du fichier executable represente par <name> dans la liste ci-dessous
make clean : nettoyage de l'archive 


Fichiers executables :

lines :
Permet de tester les fonction draw_polyline, draw_polygon, draw_text et les clipper associés

circles :
Test permettant de tester la fonction draw_tool qui dessine les widgets (button,frame et top_level)

minimal :
Prise en main de la gestion d'évenements et de la fonction hw_event_wait_next

frame :
Creation d'un widget de type frame, utilisation des fonctions du fichier ei_application et des fonctions de gestion du widget frame

button :
Creation d'un widget de type button, utilisation des fonctions du fichier ei_application et des fonctions de gestion du widget button et gestion d'evenements

hello_world : 
Creation d'un widget de type top_level, fonctions de gestion du widget top_level et gestion d'evenements et de geometrie
