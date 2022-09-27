#include "adts/node.hpp"
#include "adts/stack.hpp"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace eda;

std::string get_name(std:: string * nombre_de_donde_obtener, std::string end){
	int size_obtener= nombre_de_donde_obtener->length();
	std::string extra;
	std::string copia=*nombre_de_donde_obtener;
	if(end==".html"){
		extra=copia.erase(size_obtener-5)+".log";
	}
	if(end=="ver"){
		extra=copia.erase(0,size_obtener-5);
	}
	if (end==">"){
		extra=copia.erase(0,1);
		int size_obtener= extra.length();
		extra=extra.erase(size_obtener-1);
	}
	if(end==">."){
		extra=copia.erase(0,1);
		int size_obtener= extra.length();
		extra=extra.erase(size_obtener-1);
		extra="</"+extra;
	}
	if (end=="."){
		extra="<"+copia+">";
	}
	return extra;
}

void get_tags(std::string input, std::string *return_input, int i){
	
	std::string copia=input;
	
	for(int k1=i; k1<input.length();k1++){
		if (input[k1]=='>'){
			copia.erase(k1+1);
			break;
		}
	}
	if (i!=0){
		copia.erase(0,i);
	}
	*return_input=copia;
}


bool validatetags( std::string tag, std::string * top_tag, std::string * error_tag, std::fstream * out, Stack * stack){
		//tags de apertura
	bool error = false;
	std::string copia_tag=tag;
	std::string contenido_tag=get_name(&copia_tag,">");
	if ((contenido_tag)[0]=='/'){
		if (stack->isEmpty()){
			copia_tag=copia_tag +".";
			error = true;
			*error_tag=get_name(&copia_tag,">.");
			*top_tag= stack->top()->getData();
			return !error;
		}
		else{
			contenido_tag=contenido_tag+">";
			contenido_tag='<'+get_name(&contenido_tag, ">")+'>';
			if(stack->top()->getData() ==contenido_tag){
				stack->pop();
				*out<< "tag "<< contenido_tag <<" ok ♪♪ " << std::endl;
			}
			else{
				contenido_tag=contenido_tag +".";
				error = true;
				*error_tag=get_name(&contenido_tag,">.");
				*top_tag= stack->top()->getData();
				return !error;
			}
		}
	}
	else{
		contenido_tag=get_name(&contenido_tag,".");
		stack->push(new Node(contenido_tag));
	}
	
	
	return !error;
	}

bool print_tags(Stack * stack, std::string  input,int * line, std::string * top_tag, std::string * error_tag, std::fstream * out ){
	int i=0;
	std::string tag;
	bool error= true;
	while(i < input.length() && (error)){
		if (input[i]=='<'){
		get_tags(input, &tag, i);
		error=validatetags(tag, top_tag, error_tag, out,stack);
		}
		i++;
		
		
	}
	*line=*line+1;
	return error;
}


int main(int nargs, char** vargs){

	std::string input;	//Es la linea del texto .html leyendose
	int line=-1; 	// Nos define la linea del archivo (se inicia en -1, para que en la primera linea tenga valor = a 0, si se quiere iniciar el conteo desde 1, cambiar de -1 a 0)
	std:: string top_tag;	//Nos almacena el top del stack
	std:: string error_tag=" "; //Nos alcena el nombre del tag_que causo error
	std:: string espererado_tag; //Nos almacena el nombre del tag que se deberia haber esperado

	Stack stack;	//Es el stack del programa

	bool stat =true;	//True si no hay error, false si hay

	if (nargs==1){	//revisar que nos ingrese un archivo
		std::cout << "Debe de ingresar un archivo  para que el programa funcione"<< std::endl;
		return 0;
	}
	if (nargs==2){
		std::string nombre_archivo=vargs[1];	//ingresar el nombre del archivo .html
		std::string tipo_archivo= get_name(&nombre_archivo,"ver");	//ver el tipo del archivo

		if (tipo_archivo!=".html"){//Corroborar si es .html
			std::cout << "Debe de ingresar un archivo  .html para que el programa funcione"<< std::endl;
			return 0;
		}
		
		std:: string nombre_salida= get_name(&nombre_archivo,".html");	//nombre archivo de salida

		std::fstream file;	//archivo de entrada
		std::fstream out;	//archivo de salida
		file.open(nombre_archivo, std::ios::in);
		
		if (!file){
			std::cout<<"Error, no se puede habrir el archivo"<< nombre_archivo <<std::endl;
		}
		out.open(nombre_salida, std::ios::out);
		if (!out){
			std::cout<<"Error, no se puede habrir el archivo"<< nombre_salida <<std::endl;
		}
		
		
		while( stat){
			std::getline(file, input);
			stat= print_tags(&stack, input,&line, &top_tag, &error_tag, &out);
			if(file.eof()){
				if (!stack.isEmpty() and error_tag==" "){
					stat = false;
					error_tag="ninguno";
					top_tag=stack.top()->getData();
		}
				break;
			}
		}
		
		if (!stat){		//Evaluar cual respuesta se debe dar si es que ocurre un error
			out<< " ---- Error en la linea " ;
			out << line << std::endl;
			
			if (top_tag=="<body>"){
			espererado_tag="</body>";
			}
			if (top_tag=="<h1>"){
				espererado_tag="</h1>";
			}
			if (top_tag=="<p>"){
				espererado_tag="</p>";
			}
			if (top_tag=="<ol>"){
				espererado_tag="</ol>";
			}
			if (top_tag=="<li>"){
				espererado_tag="</li>";
			}
			if (top_tag=="<center>"){
				espererado_tag="</center>";
			}
			if (error_tag=="ninguno"){
				espererado_tag= get_name(&top_tag,">");
			}
			
			out << " Se esperaba " << espererado_tag << " en lugar de " << error_tag << std::endl;
			out << " ----"	<< std::endl;

			std::cout<< "output -> "<< nombre_salida << std::endl;
			return 0;
		}
		
		out<< " 0 Errores ♪♪♪♪" << std::endl;	//En caso de no haber errores
		
		std::cout<< "output -> "<< nombre_salida << std::endl;
		return 0;
	}
	return 0;
}
