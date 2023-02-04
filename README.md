**DESCRIPTION**

 Meteo-Project is a software created for generating graphics based on meteorological data such as temperature, pressure, wind, height, and moisture from different regions in France. The program is developed using C and shell script programming languages and utilizes GNUPLOT for graph creation. The user can initiate the program by giving a command to start processing meteorological data from a file. The software uses C functions to sort and arrange the data in specific ways, allowing for production of different graphs. The data can be filtered by time and region and the sorting order can be changed. The program offers three sorting modes for customization.



**HOW TO USE**


     1.	Download our files from this repository on your computer and put them in a directory of your choosing.
     2.	Inside your terminal, enter that directory. 3 Put your data file inside ./C/data/ (or use the one we furnish on this repository, meteo_short.csv)
     3.	Use the command "bash script.sh" with the options of your choice to start creating graphics !  Warning : some options must be used. If you have any question about how to use Meteo-Project, use the following command : "bash script.sh --help"
     4.	If you have chosen multiple options, you will be able to see the graphs one at a time. To go on to the next one, simply close the current gnuplot window.
     5.	Type "make clean" in your terminal to delete all our executables / object files if you don't want them.



**LIST OF OPTIONS**

  -f : serves to specify the path to the meteorological data file.

 --avl : data is sorted by using AVL trees. 
 
 --abr : data is sorted by using ABR.

 --tab : data is sorted by using lists.

--help : puts a help message on the terminal.

 -r : the sorts will be reversed .

-t : the program will produce graphics on temperature in French regions.
You need to specify a mode as an argument.
Use -t 1 to get average, minimum and maximum temperature for each station.
Use -t 2 to get worldly average temperatures for each date and time.
Use -t 3 to get temperature measurements for each station, date and time.


-p : the program will produce graphics on pressure in French regions.
You need to specify a mode as an argument. 
Use -p 1 to get average, minimum and maximum pressure for each station.
Use -p 2 to get worldly average pressure measurements for each date and time.
Use -p 3 to get pressure measurements for each station, date and time.

 -w : the program will produce graphics on wind in French regions. 

 -h : the program will produce graphics on the height of meteorological stations in French regions. 

-m : the program will produce graphics on moisture levels in French regions. 

-d : allows to specify a time interval from which the data used will come.  

-F : the data used will only be from the stations located in metropolitan France and Corsa.

-G : the data used will only be from stations located in French Guiana.

-A : the data used will only be from stations located in the West Indies.

-S: the data used will only be from the meteorological station of St-Pierre et Miquelon.

-Q: the data used will only be from stations located in Antarctica.

-O : the data used will only be from stations located in the Indian Ocean.


**LIST OF FILES**


•	makefile : Our makefile. It serves to compile all the C files and clean temporary files afterwards.

•	script_meteo.sh : The main shell file that executes our project entirely.

• main.c : Our main C porgram.

• help.tkt : our help file.

• Planing.pdf : Our descripyion of how our project was made .

• inculde : contains our main.h .

•exmaple_of_shart.png / example_of_compilation.png :screenshot of the result .




**LIBRARIES USED** 

  • stdio.h 
  
  • stdlib.h 
  
  • string.h



ZIDI Yanis, WOHL Nathan\ PréIng2 G4-MI \ CY TECH
