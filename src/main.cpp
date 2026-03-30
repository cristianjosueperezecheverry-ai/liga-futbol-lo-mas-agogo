#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip> // Para que la tabla se vea alineada

using namespace std;

struct Equipo {
    string nombre;
    int pj = 0, pg = 0, pe = 0, pp = 0, gf = 0, gc = 0, dg = 0, pts = 0;
};

struct Liga {
    string nombreLiga;
    int v_pts, e_pts, d_pts;
    vector<Equipo> equipos;
};

void actualizarEstadisticas(Equipo *e, int misGoles, int susGoles, int p_v, int p_e, int p_d) {
    e->pj++;
    e->gf += misGoles;
    e->gc += susGoles;
    e->dg = e->gf - e->gc;
    if (misGoles > susGoles) { e->pg++; e->pts += p_v; }
    else if (misGoles == susGoles) { e->pe++; e->pts += p_e; }
    else { e->pp++; e->pts += p_d; }
}

void cargarConfig(Liga *l) {
    ifstream arch("data/config.txt");
    string lin;
    while (getline(arch, lin)) {
        if (lin.find("nombre=") == 0) l->nombreLiga = lin.substr(7);
        if (lin.find("pts_victoria=") == 0) l->v_pts = stoi(lin.substr(13));
        if (lin.find("pts_empate=") == 0) l->e_pts = stoi(lin.substr(11));
        if (lin.find("pts_derrota=") == 0) l->d_pts = stoi(lin.substr(12));
        if (lin.find("equipo=") == 0) {
            Equipo e; e.nombre = lin.substr(7);
            l->equipos.push_back(e);
        }
    }
    arch.close();
}

void procesarTabla(Liga *l) {
    for(auto &e : l->equipos) { e.pj=0; e.pg=0; e.pe=0; e.pp=0; e.gf=0; e.gc=0; e.dg=0; e.pts=0; }
    ifstream arch("data/partidos.txt");
    string lin;
    while (getline(arch, lin)) {
        stringstream ss(lin);
        string f, loc, vis, glS, gvS;
        getline(ss,f,','); getline(ss,loc,','); getline(ss,glS,','); getline(ss,vis,','); getline(ss,gvS,',');
        int gl = stoi(glS), gv = stoi(gvS);
        for(auto &e : l->equipos) {
            if(e.nombre == loc) actualizarEstadisticas(&e, gl, gv, l->v_pts, l->e_pts, l->d_pts);
            if(e.nombre == vis) actualizarEstadisticas(&e, gv, gl, l->v_pts, l->e_pts, l->d_pts);
        }
    }
    arch.close();
    // ORDENAMIENTO PRO CON DESEMPATE [cite: 140, 199]
    sort(l->equipos.begin(), l->equipos.end(), [](const Equipo &a, const Equipo &b){
        if(a.pts != b.pts) return a.pts > b.pts;
        return a.dg > b.dg;
    });
}

void mostrarYGuardarTabla(Liga *l) {
    procesarTabla(l);
    ofstream archTabla("data/tabla.txt"); // Guarda en archivo [cite: 201]
    string cabecera = "Equipo          | PJ | PG | PE | PP | GF | GC | DG | PTS";
    cout << "\n" << l->nombreLiga << "\n" << cabecera << endl;
    archTabla << l->nombreLiga << "\n" << cabecera << endl;

    for (auto &e : l->equipos) {
        stringstream ss;
        ss << left << setw(15) << e.nombre << " | " << setw(2) << e.pj << " | " << setw(2) << e.pg << " | " 
           << setw(2) << e.pe << " | " << setw(2) << e.pp << " | " << setw(2) << e.gf << " | " 
           << setw(2) << e.gc << " | " << setw(2) << e.dg << " | " << e.pts;
        cout << ss.str() << endl;
        archTabla << ss.str() << endl;
    }
    archTabla.close();
    cout << "\n[Tabla guardada en data/tabla.txt]" << endl;
}

int main() {
    Liga miLiga;
    cargarConfig(&miLiga);
    int op = 0;
    while (op != 5) {
        cout << "\n1. Ver Tabla Pro\n2. Registrar Partido\n5. Salir\nOpcion: ";
        cin >> op;
        if (op == 1) mostrarYGuardarTabla(&miLiga);
        else if (op == 2) {
            string f, l, v; int gl, gv;
            cout << "Fecha: "; cin >> f;
            cout << "Local: "; cin.ignore(); getline(cin, l);
            cout << "Goles: "; cin >> gl;
            cout << "Visitante: "; getline(cin, v);
            cout << "Goles: "; cin >> gv;
            ofstream p("data/partidos.txt", ios::app);
            p << f << "," << l << "," << gl << "," << v << "," << gv << endl;
            p.close();
        }
    }
    return 0;
}