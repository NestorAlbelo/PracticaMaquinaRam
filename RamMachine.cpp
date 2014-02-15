#include "RamMachine.h"

//------------------------------------------------------------------------------
//--------------------------CLASE MAQUINA RAM-----------------------------------

maquinaRam::maquinaRam(){
	lineaAct=0;
}
	
//Devuelve 1 si se ha conseguido analizar el texto correctamente
bool maquinaRam::leerTexto(string nombreFichero){//Devuelve 1 si se ha conseguido analizar el texto correctamente
	char line[512];
	ifstream ifs;
	ifs.open(nombreFichero.c_str(), ifstream::in);

	while(!ifs.eof()) {
		lineaAct++;
     	ifs.getline(line,512);
     	string l(line);
     	lineas.push_back(Linea(l,lineaAct));
     	if (!lineas[lineas.size()-1].analizarLinea(*this)){
			cout << "ERROR. Ha habido un error en la linea " << lineaAct << ", por favor revise su codigo" << endl;
     		return false;
		}
   	}
	return true;
}

//Mostramos todas las lineas indicando su posicion y mostramos sus elementos
void maquinaRam::mostrarLineas(){ 
	for (int i = 0; i < lineas.size(); i++){
		cout << "Linea " << i+1 << ":" << endl;
		lineas[i].mostrarElementos();
	}
}

//Devuelve la linea donde se encuentra implementada la etiqueta "etq" y si no existe la etiqueta se devuelve un -1
int maquinaRam::getEtiquetas(string etq){
	for (int i = 0; i < etiquetas.size(); i++){
		if (etiquetas[i] == etq){ 
			return atoi(etiquetas[i+1].c_str());
		}
	}
	return -1; //No se ha encontrado ninguna etiqueta con ese identificador
}

//Añadimos al array de etiquetas una nueva con su nombre y posicion de inicio
void maquinaRam::addEtiquetas(string nombre, int pos){
	etiquetas.push_back(nombre);
	string n = pos+"";
	etiquetas.push_back(n);
}

//------------------------------------------------------------------------------
//-----------------------------CLASE LINEA--------------------------------------

maquinaRam::Linea::Linea(string line,int nline){
	lineaActual=line;
	elementos.clear();
	numeroLinea=nline;
}

//Devuelve true si el analisis fue satisfactorio
bool maquinaRam::Linea::analizarLinea(maquinaRam & object){	
	string cpyLinea = lineaActual;
	if(!cpyLinea.empty()){
		while(cpyLinea[0]==' '){												//Eliminamos los espacios posibles anteriores
			cpyLinea.erase( cpyLinea.begin() );
		}
		if(cpyLinea[0]==';'){
			setElementos(Elemento(cpyLinea,3));									//Pasamos la linea completa y le pones tipo 3 (comentario)
			return elementos[elementos.size()-1].comprobarElemento();
		}
		else{
			string temporal;
			while(cpyLinea[0]!=' '){											//Recogemos la cadena hasta encontrar un espacio
				temporal += cpyLinea[0];
				cpyLinea.erase( cpyLinea.begin() );
			}
			//------------------------------------------------------------------
			if(temporal[temporal.size()-1]==':'){								//Comprobamos si existe una etiqueta
				setElementos(Elemento(temporal,2));								//Guardamos el elemento en el vector con tipo etiqueta
				if(!elementos[elementos.size()-1].comprobarElemento()){
					return false;
				}
				temporal.erase((temporal.length())-1,1);						//Borramos los dos puntos para mantener unicamente el nombre
				object.addEtiquetas(temporal,numeroLinea);						//Lo guardamos en el vector de etiquetas
				temporal.clear();												//Limpiamos el temporal para recoger la sig cadena
				while(!cpyLinea.empty() && cpyLinea[0]==' '){					//Eliminamos los espacios posibles anteriores
					cpyLinea.erase( cpyLinea.begin() );
				}	
				while(!cpyLinea.empty() && cpyLinea[0]!=' '){					//Recogemos la cadena hasta encontrar un espacio
					temporal += cpyLinea[0];
					cpyLinea.erase( cpyLinea.begin() );
				}
			}
			//------------------------------------------------------------------
			setElementos(Elemento(temporal,0));									//Creamos el elemento con tipo de instruccion
			int halt=elementos[elementos.size()-1].comprobarElemento();
			if(!halt)
				return false;	
			//------------------------------------------------------------------
			temporal.clear();
			if (halt!=12){
				while(!cpyLinea.empty() && cpyLinea[0]==' '){					//Eliminamos los espacios posibles anteriores
					cpyLinea.erase( cpyLinea.begin() );
				}
				if (cpyLinea.empty()){											//Si la linea se vacia sin operando da error
					return false;
				}
			
				while(!cpyLinea.empty() && cpyLinea[0]!=' '){					//Recogemos la cadena hasta encontrar un espacio
					temporal += cpyLinea[0];
					cpyLinea.erase( cpyLinea.begin() );
				}		
				setElementos(Elemento(temporal,1));								//Creamos el elemento con tipo de operando
				if(!elementos[elementos.size()-1].comprobarElemento()){
					return false;
				}
			}
			
			
			//------------------------------------------------------------------
			while(!cpyLinea.empty() && cpyLinea[0]==' ' ){						//Eliminamos los espacios posibles anteriores
				cpyLinea.erase( cpyLinea.begin() );
			}	
			if(!cpyLinea.empty() && cpyLinea[0]==';'){
				setElementos(Elemento(cpyLinea,3));								//Pasamos la linea completa y le pones tipo 3 (comentario)
				return elementos[elementos.size()-1].comprobarElemento();
			}
			else if (!cpyLinea.empty())
				return false;
		}	
	}
	return true;
}

//Mostramos todos los elementos de la linea que llama a la funcion
void maquinaRam::Linea::mostrarElementos(){
	for (int i = 0; i < elementos.size(); i++)
		cout << "\tNombre: " << elementos[i].getNombre() << "\t\tTipo: " << elementos[i].getTipo() << endl;
}

void maquinaRam::Linea::setElementos(Elemento element){
	elementos.push_back(element);	
}
	
string maquinaRam::Linea::getLinea(){
	return lineaActual;
}

int maquinaRam::Linea::getNumeroLinea(){
	return numeroLinea;	
}

//------------------------------------------------------------------------------
//----------------------------CLASE ELEMENTO------------------------------------

maquinaRam::Linea::Elemento::Elemento(string n,int t){
	nombre = n;
	tipo = t;	
}

//Comprobamos la semantica del elemento devolviendo 0=error, 1=Instruccion, 2=Halt(Sin operando)
int maquinaRam::Linea::Elemento::comprobarElemento(){
	if (tipo==instruccion){
		switch (nombre){
			case: "LOAD"
				return 1;
				break;
			case: "STORE"
				return 2;
				break;
			case: "ADD"
				return 3;
				break;
			case: "SUB"
				return 4;
				break;
			case: "MULT"
				return 5;
				break;
			case: "DIV"
				return 6;
				break;
			case: "READ"
				return 7;
				break;
			case: "WRITE"
				return 8;
				break;
			case: "JUMP"
				return 9;
				break;
			case: "JGTZ"
				return 10;
				break;
			case: "JZERO"
				return 11;
				break;
			case: "HALT"
				return 12;
				break;
			default:
				break;
		}	
	}
	else if(tipo==operando){ 
		string n = nombre;
		bool esUnNumero=true;
		if (n[0]=='=' || n[0]=='*'){		//Comprobamos si se trata de un operando numerico (Inmediato o Doble direccionamiento)
			n.erase (0,1);
			for(int i=0;i<n.size();i++){
				if(!isdigit(n[i])){
					esUnNumero=false;
					break;
				}
			}
		}
		else if(nombre[0]==';' || nombre[nombre.size()-1]==':')	//Comprobamos que no empiece por ; (comentario) o termine por : (etiqueta)
			esUnNumero=false;
		if (esUnNumero && n.size()>0)
			correcto = 1;			
	}
	else if(tipo==etiqueta){
		if(nombre[nombre.size()-1]==':')
			correcto=1;
	}
	else if(tipo==comentario){
		if(nombre[0]==';')
			correcto=1;
	}
	return 0;
}


string maquinaRam::Linea::Elemento::getNombre(){
	return nombre;
}

int maquinaRam::Linea::Elemento::getTipo(){
	return tipo;	
}
