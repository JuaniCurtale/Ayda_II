#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

void resuelvo_greedy(vector<int>&indice_contratados,vector<set<string>> &empleados_contratados,set<string> &tecnologias_totales,vector<set<string>> postulantes,vector<int> &postulante_disponible){
int cantIteraciones = 0;
    while(tecnologias_totales.size() != 0){
        int pos_mayor=0;
        int cant_mayor=0;
        int noRepetidas = 0;
        int menorRepitencia = 0;

        for(int i=0; i<postulantes.size(); i++){ //itero por todos los empleados
            noRepetidas=0;
            for(auto itA = postulantes[i].begin(); itA != postulantes[i].end();itA++){ //itero por todas sus tecnologias
                if((tecnologias_totales.count(*itA)>0)){ //cuento cuantas se repiten
                    noRepetidas++;
                }

                //en la primera iteracion toma el empleado con mas tecnologias (ya que aun no cubrio ninguna tecnologia)
                if((postulantes[i].size() > cant_mayor)&&(postulante_disponible[i]==1)&&(empleados_contratados.size()==0)){
                    cant_mayor=postulantes[i].size();
                    pos_mayor=i;
                    }
                else
                    //ya en la segunda iteracion elegimos el empleado que mas tecnologias cubra de las que aun no hemos cubierto
                    if(empleados_contratados.size()!=0){
                        //quedarnos con el set que mas tecnologias cubra, que aun no este cubierto
                        if((noRepetidas>menorRepitencia)&&(postulante_disponible[i]==1)){
                            menorRepitencia = noRepetidas;
                            pos_mayor=i;
                        }
                    }
                cantIteraciones = cantIteraciones+1;
            }
        }
        postulante_disponible[pos_mayor]=0; //marcamos el empleado que cubre mas tecnologias requeridas como no disponible
        empleados_contratados.push_back(postulantes[pos_mayor]); //lo agregamos al vector de empleados
        indice_contratados.push_back(pos_mayor); //insertamos el empleado a nuestro vector donde guardamos que empleado contratamos

        //borramos de las tecnologias a cubrir, las tecnologias que cubre el empleado a insertar
        for(auto itA = postulantes[pos_mayor].begin(); itA != postulantes[pos_mayor].end(); itA++){
            if(tecnologias_totales.count(*itA)>0){
                tecnologias_totales.erase(*itA);

            }
        }

    }
    cout << endl;
    cout << "CantIteraciones Greedy: " << cantIteraciones<<endl;
    cout << "Empleados contratados con Greedy: " << endl;
    for (int i=0; i<indice_contratados.size();i++){
    cout<<indice_contratados[i]<<"|";
    }

}

void resuelvo_backtracking(int &cantIteraciones,vector<int>&indice_contratados, vector<int> & auxiliar,int postulantes_totales, int &empleados_actual, vector<set<string>> postulantes, vector<int> &postulante_disponible, vector<set<string>> &empleados_contratados, set<string> &tecnologias_totales,set<string> tecnologias_actual){
    cantIteraciones++;
    if(tecnologias_totales.size() == 0){
        if(empleados_contratados.size() < empleados_actual){ //empleados contratados.size() es la cota local
            empleados_actual = empleados_contratados.size();
            auxiliar = indice_contratados;  //actualizo la solucion
        }
    }
    else{
        int postulante = 0;

        while((postulante < postulantes_totales)&&(empleados_contratados.size()< empleados_actual))
            {
            if(postulante_disponible[postulante]== 1){
                set<string>aux2;
                //Este ciclo itera por todas las tecnologias de un postulante y si
                //alguna tecnologia pertenece al conjunto de tecnologias_totales guarda
                //esa tecnologia en aux2 y la borra de tecnologias_totales.
                for(auto itA = postulantes[postulante].begin(); itA != postulantes[postulante].end();itA++){
                    if(tecnologias_totales.count(*itA)>0){
                        aux2.insert(*itA);
                        tecnologias_totales.erase(*itA);
                    }
                }

                empleados_contratados.push_back(postulantes[postulante]);
                indice_contratados.push_back(postulante);
                postulante_disponible[postulante]= 0;
                resuelvo_backtracking(cantIteraciones,indice_contratados,auxiliar,postulantes_totales,empleados_actual,postulantes,postulante_disponible,empleados_contratados,tecnologias_totales,tecnologias_actual);
                postulante_disponible[postulante]= 1;
                indice_contratados.pop_back();
                empleados_contratados.pop_back();
                //Paso inverso al anterior ciclo for.
                for (const string &itA : aux2){
                    tecnologias_totales.insert(itA);
                    aux2.erase(itA);
                }
            }
            postulante++;
        }
    }
}

int main()
{
    //Los empleados contratados los identificamos con enteros que son los indices de nuestro vector de sets postulantes.
    //EXPLICACION DE LAS ESTRUCTURAS:
    int empleados_actual= 100000;                   //Cota Global inicializada con un valor invalido.
    set<string> tecnologias_actual;                //Set de M tecnologias que brinda la empresa.
    vector<set<string>> empleados_contratados;    //Vector de sets que guarda todos los contratados hasta el momento.
    vector<int> auxiliar;                        //Solucion final, dentro de cada auxiliar[i] hay un entero que representa a un postulante ya contratado, devuelve el vector con la menor cantidad de empleados que cubren todas las tecnologias.
    vector<int> postulante_disponible;          //0 false, 1 true, vector que nos indica si el postulante ya fue seleccionado o no.
    vector<int>indice_contratados;             //Vector que guarda los contratados hasta el momento. (solucion parcial).

    //-----------------------------PRE-CARGADO----------------------------------------------------------------------------


                                        //tecnologias ejemplo 1
	set<string> tecnologias_totales = {"css","html","c#","java","prolog","ruby","c++","fortran","javascript","python","perl","smalltalk", "php","typescript", "pascal"};; //tecnologias que brinda la empresa
                                        //tecnologias ejemplo 2
                                        //{"Fortran","Pascal","C++", "Java","Python"};;
    vector<set<string>> postulantes = {


 //Ejemplo 1
        {"html"},
        {"java","prolog","c++","fortran"},
        {"prolog","ruby","c#"},
        {"ruby", "javascript", "smalltalk"},
        {"prolog","fortran","perl","typescript"},
        {"c++","python","php","css"},
        {"php","typescript","pascal"},
        {"php","typescript","pascal","python","perl", "smalltalk"},
        {"fortran", "javascript","perl","smalltalk","typescript","pascal"},

/*    //Ejemplo 2
    {"C++", "Java", "Python"},
    {"Java", "Python"},
    {"C++", "Pascal"},
    {"Java", "Fortran"},
    {"C++", "Fortran"},
    {"C++", "Fortran","Python"},
    {"Ruby", "Fortran"},
    {"C++", "Java"},
    {"C++", "Fortran","Pascal"},
    {"C++", "Fortran"},
    {"C++", "Fortran","Pascal"},
    {"Java","C++", "Fortran"},
    {"Javascript"},
    {"C","C++", "Frontan"},
*/

    };
    for (int i=0; i<postulantes.size();i++){ //Este ciclo, inicializa el vector postulantes_disponibles.
            int valor = 1;
            postulante_disponible.push_back(valor);
        }

    //-------------------------------------------------------------------------------------------------------------------
    int cantIteraciones = 0;
	resuelvo_backtracking(cantIteraciones,indice_contratados,auxiliar,postulantes.size(),empleados_actual,postulantes,postulante_disponible,empleados_contratados,tecnologias_totales,tecnologias_actual);
    cout << "CantIteraciones Backtracking: " << cantIteraciones << endl;
    cout<<"---EMPLEADOS CONTRATADOS CON BACKTRACKING---"<<endl;
    for (int i=0; i<auxiliar.size();i++){
        cout<<auxiliar[i]<<"|";
    }

    cout << endl;

    indice_contratados.clear(); //seteo el indice de contratados para utilizarlo ahora con greedy
	resuelvo_greedy(indice_contratados,empleados_contratados,tecnologias_totales,postulantes,postulante_disponible);


    //cout<<"Cantidad de contratados --> "<< auxiliar.size() <<endl;


    return 0;
}
