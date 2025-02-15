#include "pch.h"
//g++  5.4.0

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <time.h>
#include <vecters.h>
#include <Gas.h> 
#include <Mags_and_coils.h>
using namespace std;

#define MIN_ERROR_IN_VECTOR 1


// вектор из пар вектеров vector<vecter, vecter>
// Из того, который прикреплен к сенсеру вычитаем данный полем
// Составить, то какая бы сатрица была если бы координата магнита была такая
// x1, x2 составить матрицу векторов, для каждого


struct Sensor {
    vecter coordinates;
    vecter value;
};


//IT WORKS ONLY IF POINT (0, 0, 0) IS IN OUR AREA AND ALSO IF IT IS A POINT!!!! OR (THE SAME) IF X AND Y AXIS HAVE (2*N + 1) POINTS
vector<vector<vecter>> SearchingCommonArea(vector<Electromagnet>& static_magnets) {		//!!!!BH_correct
    double overlap_left = 0;
    double overlap_right = 0;
    for (int i = 0; i < static_magnets.size(); ++i) {
        double x_left = static_magnets[i].position.x_proj - static_magnets[i].Mag_field.size_x;
        double x_right = static_magnets[i].position.x_proj + static_magnets[i].Mag_field.size_x;
        if (overlap_left < x_left || i == 0){
            overlap_left = x_left;
        }
        if (overlap_right > x_right || i == 0){
            overlap_right = x_right;
        }
    }
	
    vecter zero_vecter(0, 0, 0);
    int length_x = static_cast<int>((overlap_right - overlap_left));
    int length_y = static_magnets[0].Mag_field.size_y * 2;
    vector<vector<vecter>> area(length_x, vector<vecter>(static_magnets[0].Mag_field.size_y, zero_vecter));		//!!!!BH_correct
    
    for (int mag = 0; mag < static_magnets.size(); ++mag) {
        if (fabs(static_magnets[mag].position.x_proj) - static_cast<int>(fabs(static_magnets[mag].position.x_proj)) < MIN_ERROR_IN_VECTOR) {
            
		//NEED TO WORK WITH OVERLAP FOR EACH MAGNET HERE
		
		///////////////////////////////////
		
		// NEW CIRCLES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		//////////////////////////////////
		
		for (int y = -length_y / 2; y <= length_y / 2; ++y) { // may be static_cast too?
			for (int x = static_cast<int>(max(overlap_left, static_magnets[mag].position.x_proj - static_magnets[mag].Mag_field.size_x)); 
			     x <= static_cast<int>(min(overlap_right, static_magnets[mag].position.x_proj + static_magnets[mag].Mag_field.size_x));
			     ++x) {
				int local_x = x + static_cast<int>(length_x / 2); // coordinate in common field
				int local_y = y + static_cast<int>(length_y / 2); // ????????? Не уверен насчет направления поля (вверх\вниз). 
				// Тип не ясно, откуда считаем наше поле и нужно ли переворчаивать все y-вектора от магнитов и катушек?
				
				if (local_x < 0) {
					cout << "ERROR IN SEARCH_POSITION: FIELD IS NOT SYMMETRIC AND WE GET MINUS-COORDINATES (local_x = "
						<< local_x << "\n mag = " << mag << "\n x = " << x << endl;
					system("pause");
				}
				
				//area[local_x][local_y].z_proj += static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.z_proj;
				//z-coordinate is not needed ?
				
				if (x < 0) {
					area[local_x][local_y].x_proj += 
						static_magnets[mag].Mag_field.cells[-x][abs(y)].Mag_vec.x_proj * (-1);
				} else {
					area[local_x][local_y].x_proj += 
						static_magnets[mag].Mag_field.cells[x][abs(y)].Mag_vec.x_proj;
				}
				if (y < 0) {
					area[local_x][local_y].y_proj += 
						static_magnets[mag].Mag_field.cells[abs(x)][-y].Mag_vec.y_proj * (-1);
				} else {
					area[local_x][local_y].y_proj += 
						static_magnets[mag].Mag_field.cells[abs(x)][y].Mag_vec.y_proj;
				}
			}
		}
		
		///////////////////////////////////////////////
		
		// THE END OF NEW CODE!!!!
		
		///////////////////////////////////////////////
		
		
		for (int y = 0; y < length_y; ++y) {
                for (int x = 1; x < length_x - 1; ++x) {
                    int local_x = abs(x - static_cast<int>(static_magnets[mag].position.x_proj) - length_x / 2);
                    area[x][y].z_proj += 
                        static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.z_proj;
                    area[x][y].x_proj += 
                        static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.x_proj * 
                        sign(static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.y_proj);   //UNCORRECT
                    area[x][y].y_proj += 
                        static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.y_proj * 
                        sign(static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.x_proj);	//UNCORRECT
                    // Not exist in 3/4 part of area. We need to look it for from 1/4 part of area. DONE! 
                    // IS SIGN WORK AS I WANT??? (-1, 0, 1) ???????????????	           /// YES
                    // ATTENTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    // ATTENTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    // I AM NOT SURE, IF THIS CODE WORKS IN GOOD WAY WITH ZERO-COORDINATES, 
                    // FOR EXAMPLE (X, 0) OR (0, Y). 
                    // IN THESE COORDINATES SIGN CAN MAKE VECTORS TO (0, 0) AND THIS IS UNCORRECT!!!!!!!!!!!!!!!
                    // MAY BE WE NEEDN'T WORRY ABOUT IT, BECAUSE THE REAL ZERO (0) IN DOUBLE-FORMAT IS DIFFICULT TO GET.
                    // THANK YOU, AFTER YOU WILL THINK ABOUT IT AND GET NEW COMMENTS AND CORRECTIONS IN CODE!
                    // IF YOU HAVE QUESTIONS, WRITE HERE: denis.mitr99@mail.ru
                    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!????????????
                }
            }
        } else {
            double right_variation = fabs(static_magnets[mag].position.x_proj) - static_cast<int>(fabs(static_magnets[mag].position.x_proj));
            double left_variation = 1 - right_variation;
            for (int y = 0; y < length_y; ++y) {
                for (int x = 1; x < length_x - 1; ++x) {
                    int local_x = 0; // The nearest point to (x, 0, 0) in our Area. Will be got below.
                    if (static_magnets[mag].position.x_proj > 0) {
                        local_x = abs(x - static_cast<int>(static_magnets[mag].position.x_proj) - length_x / 2);
                        
                        area[x][y].z_proj += 
                            static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.z_proj;

                        area[x][y].x_proj += 
                            static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.x_proj * 
                            sign(static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.y_proj) * right_variation + //UNCORRECT
                            static_magnets[mag].Mag_field.cells[local_x - 1][y].Mag_vec.x_proj * 
                            sign(static_magnets[mag].Mag_field.cells[local_x - 1][y].Mag_vec.y_proj) * left_variation;//UNCORRECT

                        area[x][y].y_proj += 
                            static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.y_proj * 
                            sign(static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.x_proj * right_variation + //UNCORRECT
                            static_magnets[mag].Mag_field.cells[local_x - 1][y].Mag_vec.x_proj * left_variation);
                        
                    } else {
                        local_x = abs(x - static_cast<int>(static_magnets[mag].position.x_proj) - length_x / 2 + 1);
                        // (+ 1) in the end because of rounding of static_cast in less side
                        
                        area[x][y].z_proj += 
                            static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.z_proj;
                        
                        area[x][y].x_proj += 
                            static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.x_proj * 
                            sign(static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.y_proj) * left_variation + 
                            static_magnets[mag].Mag_field.cells[local_x + 1][y].Mag_vec.x_proj * 
                            sign(static_magnets[mag].Mag_field.cells[local_x + 1][y].Mag_vec.y_proj) * right_variation;

                        area[x][y].y_proj += 
                            static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.y_proj * 
                            sign(static_magnets[mag].Mag_field.cells[local_x][y].Mag_vec.x_proj * left_variation + 
                            static_magnets[mag].Mag_field.cells[local_x + 1][y].Mag_vec.x_proj * right_variation);
                            
                        // ATTENTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        // WE ALSO HAVE SUCH PROBLEMS WITH SIGN HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        // ATTENTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        
                    }
                }
            }
        }
    }
    return area;
}


pair<double, double> First_Search_positition_magnet(vector<vector<vecter>>& area, vector<Magnet>& action_magnets,		//!!!!BH_correct
                                                    vector<Sensor>& sensors, vector<vector<vector<vecter>>>& MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS
                                                   /*NEED TO MAKE AND SAVE THIS MATRIX FOR THE NEXT ITERATIONS!!!!!*/){
    vector<vecter> sensors_coordinates_in_area(sensors.size());
    
    int x_length = area.size();
    int y_length = area[0].size();
    int x_dist = area.size() / 2;
    int y_dist = area[0].size() / 2;
    
    for (int i = 0; i < sensors.size(); ++i) {
        // (x_dist + x, y_dist - y, 0) - coordinates to get area's analog of vecter with sensor's coordinates
        sensors_coordinates_in_area[i].x_proj = x_dist + sensors[i].coordinates.x_proj;
        sensors_coordinates_in_area[i].y_proj = y_dist - sensors[i].coordinates.y_proj;
        sensors_coordinates_in_area[i].z_proj = sensors[i].coordinates.z_proj;
    }
    
    vector<vecter> sensors_without_static_area(sensors.size());
    for (int i = 0; i < sensors.size(); ++i){
        sensors_without_static_area[i] = sensors[i].value - area[sensors_coordinates_in_area[i].x_proj][sensors_coordinates_in_area[i].y_proj];
    }
    
    //Create matrix of all places of magnets for each vecter in "sensors_without_static_area" to get the nearest vecter
    vector<vector<vector<vecter>>> magnets_places(sensors_without_static_area.size(), vector<vector<vecter>>(x_length, vector<vecter>(x_length)));
    for (int i = 0; i < sensors_without_static_area.size(); ++i) {   
        //vector<vector<vecter>> magnets_places(x_length, vector<vecter>(x_length));
        for (int first_magnet = 0; first_magnet < x_length; ++first_magnet) {
            for (int second_magnet = 0; second_magnet < x_length; ++second_magnet) {
                //Get vecter from area from both magnets in this position (need to convert in new center (0, 0, 0))
                magnets_places[i][first_magnet][second_magnet] = 
                    action_magnets[0].Mag_field.cells[sensors_coordinates_in_area[i].x_proj - (first_magnet - x_dist)][sensors_coordinates_in_area[i].y_proj].Mag_vec + 
                    action_magnets[1].Mag_field.cells[sensors_coordinates_in_area[i].x_proj - (second_magnet - x_dist)][sensors_coordinates_in_area[i].y_proj].Mag_vec;
            }
        }
    }
    // NEED TO SAVE FOR THE NEXT ITERATIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS = magnets_places; // !!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    //`````````````````````````````````````````````````````````````````````````````
    // + A* - algorithm for each founding of the nearest vecter
    // We get aproximately the same results after getting coordinates of magnets from 1st iteration. 
    // We needn't do this algorithm on other iterations, because we will have a little bit different vecters in the nearest neighborhood.
    
    double fst_magnet_total_x;
    double snd_magnet_total_x;
    vector<int> fst_magnet_x(magnets_places.size());
    vector<int> snd_magnet_x(magnets_places.size());
    
    for (int i = 0; i < magnets_places.size(); ++i) {
        vecter curr_vec(0, 0, 0);
        //queue<vecter> nearest_points;
        
        // Idea: get center point and from it choose the shortest way to our point (answer) with the help of evristic of distance. 
        // If I need bool matrix? OR my matrix (magnets_place[i]) is guarantee linear?
        // vector<vector<bool>> is_used(x_length, vector<bool>(x_length, false); // NEED?????????
        
        //while (/*DO I HAVE func ABS IN VECTERS?*/ abs(curr_vec - sensors_without_static_area[i]) < MIN_DISTANCE_BTW_VECTERS) {
        //    //DO SMTH
        //}
        
        bool is_found = false;
        double min_distance_in_iteration = MAX_DOUBLE;
        for (int first_magnet = 0; first_magnet < x_length; ++first_magnet) {
            for (int second_magnet = 0; second_magnet < x_length; ++second_magnet) {
                double abs_vecters = (magnets_places[i][first_magnet][second_magnet] - sensors_without_static_area[i]).len();
                if (/*DO I HAVE func ABS IN VECTERS?*/abs_vecters < MIN_DISTANCE_BTW_VECTERS) { // Yes: .len() //!!!!BH_correct
                    is_found = true;
                    min_distance_in_iteration = abs_vecters;
                    fst_magnet_x[i] = first_magnet;
                    snd_magnet_x[i] = second_magnet;
                    break;
                } else {
                    if (min_distance_in_iteration > abs_vecters) {
                        min_distance_in_iteration = abs_vecters;
                    }
                }
            }
            if (is_found) {
                break;
            }
        }
        if (!is_found){
            cerr << "ATTENTION!!! ERROR : Vecter is not found in matrix. (31.07.2019)\n :( \n Hello, Boris, Denis, Anton, Maks and other! Dolbimsya!" << endl;
            cerr << "Function: First_Search_positition_magnet\n";
            cerr << "The nearest vecter has distance " << min_distance_in_iteration << endl;
            System("pause");
        }
    }
    
    pair<double, double> total = {0, 0};
    for (int i = 0; i < magnets_places.size(); ++i) {
        total.first += fst_magnet_x[i];
        total.second += snd_magnet_x[i];
    }
    total.first /= magnets_places.size();
    total.first -= x_dist;
    total.second /= magnets_places.size();
    total.second -= x_dist;
    return total;
}

bool Compare(pair<pair<int, int>, double> left, pair<pair<int, int>, double> right){
    return left.second > right.second;
}

pair<pair<int, int>, double> create_elem_for_queue(int fst_magnet, int snd_magnet, double sub_distance){
    pair<int, int> our_coordinate (fst_magnet, snd_magnet);
	pair<pair<int, int>, int> para(our_coordinate, sub_distance);		//!!!!BH_correct
	return para;														//!!!!BH_correct
}

pair<double, double> Search_positition_magnet(double dtime, vector<vector<vecter>>& area, vector<Electromagnet>& magnets, vector<Sensor>& sensors,			//!!!!BH_correct
                                             vector<vector<vector<vecter>>>& MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS) {
	vector<vecter> sensors_coordinates_in_area(sensors.size());
	int x_length = area.size();
	int y_length = area[0].size();
    int x_dist = area.size() / 2;
    int y_dist = area[0].size() / 2;
    
    for (int i = 0; i < sensors.size(); ++i) {
        // (x_dist + x, y_dist - y, 0) - coordinates to get area's analog of vecter with sensor's coordinates
        sensors_coordinates_in_area[i].x_proj = x_dist + sensors[i].coordinates.x_proj;
        sensors_coordinates_in_area[i].y_proj = y_dist - sensors[i].coordinates.y_proj;
        sensors_coordinates_in_area[i].z_proj = sensors[i].coordinates.z_proj;
    }
    
    vector<vecter> sensors_without_static_area(sensors.size());
    for (int i = 0; i < sensors.size(); ++i){
        sensors_without_static_area[i] = sensors[i].value - area[sensors_coordinates_in_area[i].x_proj][sensors_coordinates_in_area[i].y_proj];
    }
    
    
    int delta_fst_magnet = static_cast<int>((fabs(magnets[0].speed.x_proj) + fabs(magnets[0].accel.x_proj) * dtime) * dtime); // MAX position changing of fst magnet during dtime
    int delta_snd_magnet = static_cast<int>((fabs(magnets[1].speed.x_proj) + fabs(magnets[1].accel.x_proj) * dtime) * dtime); // MAX position changing of snd magnet during dtime
    
    vector<vector<bool>> is_visit(x_length, vector<bool>(x_length, false));
        
    // We needn't to do new matrix.
    //vector<vector<vector<vecter>>> magnets_places(sensors_without_static_area.size(), vector<vector<vecter>>(delta_fst_magnet, vector<vecter>(delta_snd_magnet)));
    
    int num_of_all_cells = x_length * x_length;
    
    vector<int> fst_magnet_x(MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS.size());
    vector<int> snd_magnet_x(MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS.size());
    
    
    for (int i = 0; i < MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS.size(); ++i) {
        priority_queue<pair<pair<int, int>, double>, vector<pair<pair<int, int>, double>>, bool(*)(pair<pair<int, int>, double>, pair<pair<int, int>, double>)> vecters_queue (Compare);
        int fst_magnet = static_cast<int>(magnets[0].position.x_proj + x_dist);
        int snd_magnet = static_cast<int>(magnets[1].position.x_proj + x_dist);
        
        vecters_queue.push(create_elem_for_queue(fst_magnet, 
                                                 snd_magnet, 
                                                 (MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS[i][fst_magnet][snd_magnet] - sensors_without_static_area[i]).len()));	//!!!!BH_correct
        int num_visit_cells = 0;
        //is_visit[fst_magnet][snd_magnet] = true;
	pair<pair<int, int>, double> curr_position = vecters_queue.top();
	while (!vecters_queue.empty() || num_of_all_cells != num_visit_cells || vecters_queue.top().second > MIN_DISTANCE_BTW_VECTERS) {
	curr_position = vecters_queue.top();
            vecters_queue.pop();
            is_visit[curr_position.first.first][curr_position.first.second] = true;
            ++num_visit_cells;
	    for (int insert_elem_index = max(curr_position.first.first - 1, 0); insert_elem_index < min(length_x, curr_position.first.first + 1); ++insert_elem_index) { //!!!!BH_correct// where is length_x?
	    for (int insert_elem_index_y = max(curr_position.first.second - 1, 0), insert_elem_index_y < min(length_x, curr_position.first.second + 1); ++insert_elem_index_y) {
			if (!is_visit[insert_elem_index][insert_elem_index_y]) {
                        vecters_queue.push(create_elem_for_queue(insert_elem_index, 
                                                                 insert_elem_index_y, 
                                                                 (MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS[i][insert_elem_index][insert_elem_index_y] - sensors_without_static_area[i]).len()));	//!!!!BH_correct
                    }
                }
            }
            
        }
        if (!vecters_queue.empty() && vecters_queue.top().second < MIN_DISTANCE_BTW_VECTERS) {
            fst_magnet_x[i] = vecters_queue.top().first.first;		// That's ok?
            snd_magnet_x[i] = vecters_queue.top().first.second;
        } else {
            cerr << "ATTENTION!!! ERROR : Vecter is not found in matrix. (31.07.2019)\n :( \n Hello, Boris, Denis, Anton, Maks and other! Dolbimsya!!!!" << endl;
            cerr << "Function: Search_positition_magnet\n";
            cerr << "Amount of visited cells = " << num_visit_cells << endl;
            System("pause");
        }
        
    }
    
    pair<double, double> total = {0, 0};
    for (int i = 0; i < MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS.size(); ++i) {
        total.first += fst_magnet_x[i];
        total.second += snd_magnet_x[i];
    }
    
    total.first /= MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS.size();
    total.first -= x_dist;
    total.second /= MATRIX_OF_ALL_MAGNETS_POSITIONS_FOR_ALL_HALL_SENSORS.size();
    total.second -= x_dist;
    return total;
}
