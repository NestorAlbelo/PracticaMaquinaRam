#ifndef __RAMMACHINE_H__
#define __RAMMACHINE_H__

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h> 
using namespace std;


enum Tipo {instruccion, operando, etiqueta, comentario};

//Clase para almacenar todo el texto--------------------------------------------
class maquinaRam {
	private:
		//Clase para la linea del texto-----------------------------------------
		class Linea{
			private:
				//Clase para los elementos individuales ------------------------
				class Elemento{
					private:
						string nombre;
						int tipo;
					public:
						//Metodos de creacion de los elementos y comprobacion de que los elementos son validos semanticamente
						Elemento(string n,int t);
						int comprobarElemento();
						string getNombre();
						int getTipo();
				};
				//--------------------------------------------------------------
				string lineaActual;
				vector <Elemento> elementos;
				int numeroLinea;
			public:
				//Metodos de analizacion de la linea pasada
				Linea(string linea,int line);
				bool analizarLinea(maquinaRam & object); //Devuelve 1 si el analisis fue satisfactorio
				void mostrarElementos();
				void setElementos(Elemento element);
				string getLinea();
				int getNumeroLinea();
		};
		//----------------------------------------------------------------------
		vector <Linea> lineas;
		int lineaAct;
		vector <string> etiquetas;
	public:
		//Metodo de obtencion de lineas
		maquinaRam();
		void mostrarLineas();
		bool leerTexto(string nombreFichero); //Devuelve 1 si se ha conseguido analizar el texto correctamente
		int getEtiquetas(string etq);
		void addEtiquetas(string nombre, int pos);
};
//------------------------------------------------------------------------------


	

#endif
