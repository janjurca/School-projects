/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2015/16
 * a pro dokumentaci Javadoc.
 */

/**
 * @defgroup Definice_Struktur
 * @{
 */

/**
 * Struktura reprezentujici jeden bod v soustave souradnic
 * @brief The obj_t struct
 */
struct obj_t {
    /// identifikacni cislo objektu
    int id;
    /// souradnice x
    float x;
    /// souradnice y
    float y;
};

/**
 * Shluk struktur obj_t;
 * @brief The cluster_t struct
 */
struct cluster_t {
    /// pocet obejktu v shluku
    int size;
    /// alokovany pocet objektu
    int capacity;
    /// ukazatel na pole objektu
    struct obj_t *obj;
};

/**
 * @}
 */

/**
 * @defgroup Prace_se_shluky
 * @{
 */

/**
 * Inicializuje shluk 'c'. Alokuje pamet pro urcity pocet bodu ve shluku.
 * @brief init_cluster
 * @param c ukazatel na shluk
 * @param cap pocet shluku pro ktere se ma alokovat misto
 * @pre poterbuje korektni ukazatel *c na shluk
 * @post alokuje pamet pro urcity pocet objektu ve shluku
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * Odstrani vsechny objekty shluku 'c' a vynuluje velikost a kapacitu
 * @brief clear_cluster
 * @param c ukazatel na shluk
 * @post dealokuje pole objektu ve shluku
 */
void clear_cluster(struct cluster_t *c);

/**
 * Doporuceny pocet novych objektu ve shluku.
 * @brief CLUSTER_CHUNK
 */
extern const int CLUSTER_CHUNK;

/**
 * Zmeni velikost shluku.
 * @brief resize_cluster
 * @param c ukazatel na shluk
 * @param new_cap nova kapacita
 * @return ukazatel na shluk
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Na konec shluku prida objekt.
 * @brief append_cluster
 * @param c ukazatel na shluk do ktereho se ma pridat
 * @param obj objekt ktery se ma pridat
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Spoji dva shluky.
 * @brief merge_clusters
 * @param c1 shluk do ktereho se pridavaji objekty
 * @param c2 shluk ze ktereho se kopiruji objekty
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Odstrani shluk z pole shluku
 * @brief remove_cluster
 * @param carr ukazatel na prvni polozku pole shluku
 * @param narr pocet shluku v poli
 * @param idx  index shluku pro odstraneni
 * @return novy pocet shluku v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @}
 */

/**
 * @defgroup Pocitani_s_objekty
 * @{
 */

/**
 * Pocita Euklidovskou vzdalenost mezi dvema objekty.
 * @brief obj_distance
 * @param o1 ukazatel na prvni objekt
 * @param o2 ukazatel na druhy objekt
 * @return vzdalenost mezi objekty
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Pocita vzdalenost dvou shluku podle nejvzdalenejsiho bodu.
 * @brief cluster_distance
 * @param c1 prvni shluk
 * @param c2 druhy shluk
 * @return vzdalenost shluku
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Hleda dva nejblizsi shluky v poli shluku.
 * @brief find_neighbours
 * @param carr ukazatel na prvni prvek pole shluku
 * @param narr pocet prvku pole shluku
 * @param c1 index jednoho blizkeho shluku
 * @param c2 index druheho blizkeho shluku
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @}
 */

/**
 * Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 * @brief sort_cluster
 * @param c ukazatel na prvni prvek pole shluku
 */
void sort_cluster(struct cluster_t *c);

/**
 * Tiskne objekty jednoho shluku.
 * @brief print_cluster
 * @param c ukazatel na shluk
 */
void print_cluster(struct cluster_t *c);

/**
 * Ze souboru nacte objekty a priradi vzdy jeden objekt do jednoho shluku.
 * @brief load_clusters
 * @param filename nazev souboru s objekty
 * @param arr ukazatel na prvni prvek pole shluku
 * @return pocet shluku v poli
 * @post **arr bude ukazovat na pole shluku
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Tiskne shluky s pole shluku.
 * @brief print_clusters
 * @param carr ukazatel na prvni prvek pole shluku
 * @param narr  pocet shluku k tisknuti
 */
void print_clusters(struct cluster_t *carr, int narr);
