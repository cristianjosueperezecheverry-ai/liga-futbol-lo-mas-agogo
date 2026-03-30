#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Estructura para representar a cada equipo
struct Equipo {
    string nombre;
    int puntos = 0;
};

// Estructura principal de la Liga
struct Liga {
    string nombreLiga;
    vector<Equipo> listaEquipos;
};

// Función que usa PUNTEROS para cargar la configuración
void cargarConfiguracion(Liga *l) { 
    ifstream archivo("data/config.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir data/config.txt" << endl;
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        if (linea.find("nombre=") == 0) l->nombreLiga = linea.substr(7);
        if (linea.find("equipo=") == 0) {
            Equipo nuevo;
            nuevo.nombre = linea.substr(7);
            l->listaEquipos.push_back(nuevo);
        }
    }
    archivo.close();
}

// Función para registrar un partido en el archivo de texto
void registrarPartido() {
    string fecha, local, visitante;
    int golesL, golesV;

    cout << "\n--- Registrar Partido ---" << endl;
    cout << "Fecha (DD/MM): "; cin >> fecha;
    cout << "Equipo Local: "; cin.ignore(); getline(cin, local);
    cout << "Goles " << local << ": "; cin >> golesL;
    cout << "Equipo Visitante: "; cin.ignore(); getline(cin, visitante);
    cout << "Goles " << visitante << ": "; cin >> golesV;

    // Guardar en ARCHIVO (Modo append para no borrar lo anterior)
    ofstream archivo("data/partidos.txt", ios::app);
    if (archivo.is_open()) {
        archivo << fecha << "," << local << "," << golesL << "," << visitante << "," << golesV << endl;
        archivo.close();
        cout << "¡Partido guardado con éxito!" << endl;
    }
}

int main() {
    Liga miLiga;
    cargarConfiguracion(&miLiga); // Pasamos la dirección de memoria (Puntero)

    int op = 0;
    while (op != 5) {
        cout << "\n=== " << miLiga.nombreLiga << " ===" << endl;
        cout << "1. Ver Tabla de Posiciones\n2. Registrar Partido\n5. Salir\nOpcion: ";
        cin >> op;

        if (op == 1) {
            cout << "\n--- Clasificación Actual ---" << endl;
            for (const auto& e : miLiga.listaEquipos) {
                cout << e.nombre << ": " << e.puntos << " pts" << endl;
            }
        } else if (op == 2) {
            registrarPartido();
        }
    }
    return 0;
}