//
// Created by arche on 4.07.2025.
//

#ifndef CITY_INFO_H
#define CITY_INFO_H
#include <vector>
#include <string>
#include <glm/vec3.hpp>
 // Город 1 - существующий Gavnar
    std::vector<glm::vec3> gavnarPoints = {
        glm::vec3(-60.0f, 0.0f, 51.0f),   // A (0)
        glm::vec3(-30.0f, 0.0f, 45.0f),   // B (1)
        glm::vec3(-5.0f, 0.0f, 30.0f),    // C (2)
        glm::vec3(35.0f, 0.0f, -10.0f),   // D (3)
        glm::vec3(110.0f, 0.0f, -15.0f),  // E (4)
        glm::vec3(150.0f, 0.0f, 40.0f),   // F (5)
        glm::vec3(-115.0f, 0.0f, -45.0f), // G (6)
        glm::vec3(-70.0f, 0.0f, -55.0f),  // H (7)
        glm::vec3(-20.0f, 0.0f, -25.0f),  // I (8)
        glm::vec3(75.0f, 0.0f, 45.0f),    // J (9)
        glm::vec3(130.0f, 0.0f, 75.0f),   // K (10)
        glm::vec3(60.0f, 0.0f, 125.0f),   // L (11)
        glm::vec3(30.0f, 0.0f, 85.0f),    // M (12)
        glm::vec3(-15.0f, 0.0f, -105.0f), // N (13)
        glm::vec3(-60.0f, 0.0f, -160.0f), // O (14)
    };

    std::vector<std::vector<int>> gavnarRoutes = {
        {0, 1, 2, 3, 4, 5},
        {6, 7, 8, 3, 9, 10},
        {11, 12, 2, 8, 13, 14},
    };

    std::vector<std::string> gavnarStations = {
        "Gavnar", "North Terminal", "East Junction", "South Gate", "West End",
        "Final Stop", "Industrial Park", "Riverside", "Mountain View",
        "Green Park", "UAM (kazn perdezhom)", "Sports Complex",
        "Theater District", "Museum Quarter", "Airport Terminal"
    };

    // Город 2 - новый Metro City
    std::vector<glm::vec3> metroPoints = {
        glm::vec3(-80.0f, 0.0f, 80.0f),   // 0
        glm::vec3(0.0f, 0.0f, 90.0f),     // 1
        glm::vec3(80.0f, 0.0f, 60.0f),    // 2
        glm::vec3(100.0f, 0.0f, 0.0f),    // 3
        glm::vec3(60.0f, 0.0f, -80.0f),   // 4
        glm::vec3(-20.0f, 0.0f, -100.0f), // 5
        glm::vec3(-90.0f, 0.0f, -40.0f),  // 6
        glm::vec3(-70.0f, 0.0f, 20.0f),   // 7
        glm::vec3(20.0f, 0.0f, 40.0f),    // 8
        glm::vec3(40.0f, 0.0f, -20.0f),   // 9
    };

    std::vector<std::vector<int>> metroRoutes = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 3, 5},
    };

    std::vector<std::string> metroStations = {
        "Metro Center", "North Plaza", "East Tower", "Commerce Hub",
        "South Port", "Industrial Zone", "West Quarter", "Park Avenue",
        "City Square", "Business District"
    };

    // Город 3 - Valley Network
    std::vector<glm::vec3> valleyPoints = {
        glm::vec3(-120.0f, 0.0f, 0.0f),   // 0
        glm::vec3(-60.0f, 0.0f, 40.0f),   // 1
        glm::vec3(0.0f, 0.0f, 20.0f),     // 2
        glm::vec3(60.0f, 0.0f, 60.0f),    // 3
        glm::vec3(120.0f, 0.0f, 20.0f),   // 4
        glm::vec3(80.0f, 0.0f, -40.0f),   // 5
        glm::vec3(20.0f, 0.0f, -80.0f),   // 6
        glm::vec3(-40.0f, 0.0f, -60.0f),  // 7
    };

    std::vector<std::vector<int>> valleyRoutes = {
        {0, 1, 2, 3, 4},
        {5, 6, 7, 0},
    };

    std::vector<std::string> valleyStations = {
        "Valley Station", "Mountain View", "Central Valley",
        "Highland Park", "East Ridge", "Canyon Point",
        "River Crossing", "West Valley"
    };


    std::vector<glm::vec3> warsawPoints = {
    // M1 (0-20)
    glm::vec3(-50, 0, 120), glm::vec3(-45, 0, 100), glm::vec3(-40, 0, 85),
    glm::vec3(-35, 0, 70), glm::vec3(-30, 0, 55), glm::vec3(-25, 0, 40),
    glm::vec3(-20, 0, 25), glm::vec3(-15, 0, 10), glm::vec3(0, 0, 0),
    glm::vec3(5, 0, -15), glm::vec3(10, 0, -30), glm::vec3(15, 0, -45),
    glm::vec3(20, 0, -60), glm::vec3(25, 0, -75), glm::vec3(30, 0, -90),
    glm::vec3(35, 0, -105), glm::vec3(40, 0, -120), glm::vec3(45, 0, -135),
    glm::vec3(50, 0, -150), glm::vec3(55, 0, -165), glm::vec3(60, 0, -180),

    // M2 (21-38)
    glm::vec3(-65, 0, 35), glm::vec3(-55, 0, 35), glm::vec3(-45, 0, 35),
    glm::vec3(-35, 0, 30), glm::vec3(-25, 0, 20), glm::vec3(-10, 0, 5),
    glm::vec3(0, 0, 0),     // Swietokrzyska (shared with M1)
    glm::vec3(10, 0, -5), glm::vec3(20, 0, -5), glm::vec3(30, 0, -5),
    glm::vec3(40, 0, -5), glm::vec3(50, 0, -5), glm::vec3(60, 0, 0),
    glm::vec3(70, 0, 5), glm::vec3(80, 0, 10), glm::vec3(90, 0, 15),
    glm::vec3(100, 0, 20), glm::vec3(110, 0, 25),

    // M3 (39-46)
    glm::vec3(0, 0, 10), glm::vec3(5, 0, 20), glm::vec3(10, 0, 30),
    glm::vec3(15, 0, 40), glm::vec3(20, 0, 50), glm::vec3(25, 0, 60),
    glm::vec3(30, 0, 70), glm::vec3(35, 0, 80),

    // M4 (47-55)
    glm::vec3(-20, 0, -10), glm::vec3(-30, 0, -20), glm::vec3(-40, 0, -30),
    glm::vec3(-50, 0, -40), glm::vec3(-60, 0, -50), glm::vec3(-70, 0, -60),
    glm::vec3(-80, 0, -70), glm::vec3(-90, 0, -80), glm::vec3(-100, 0, -90)
    };

    std::vector<std::vector<int>> warsawRoutes = {
        // M1 (0–20)
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
        // M2 (21–38)
        {21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38},
        // M3 (39–46)
        {39, 40, 41, 42, 43, 44, 45, 46},
        // M4 (47–55)
        {47, 48, 49, 50, 51, 52, 53, 54, 55}
    };

    std::vector<std::string> warsawStations = {
        // M1
        "Mlociny", "Wawrzyszew", "Stare Bielany", "Slodowiec", "Marymont", "Plac Wilsona",
        "Dworzec Gdanski", "Ratusz Arsenal", "Swietokrzyska", "Centrum", "Politechnika",
        "Pole Mokotowskie", "Raclawicka", "Wierzbno", "Wilanowska", "Sluzew",
        "Ursynow", "Stoklosy", "Imielin", "Natolin", "Kabaty",

        // M2
        "Karolin", "Chrzanow", "Lazurowa", "Bemowo", "Ksiecia Janusza", "Mlynow",
        "Swietokrzyska", "Nowy Swiat-Uniwersytet", "Centrum Nauki Kopernik",
        "Stadion Narodowy", "Dw. Wschodni", "Targowek Mieszkaniowy",
        "Trocka", "Zacisze", "Kondratowicza", "Bródno", "STP Kozia Gorka", "Brodno",

        // M3
        "Goclaw", "Ostrobramska", "Rondo Wiatraczna", "Minska", "Jana Nowaka-Jezioranskiego",
        "Sielce", "Dolina", "Gosciniec",

        // M4
        "Zwirki i Wigury", "Wislicka", "Cmentarz Zydowski", "Plac Narutowicza",
        "Bitwy Warszawskiej 1920r", "Plocka", "Okopowa", "Rondo Daszynskiego",
        "STP Polfa"
    };


std::vector<glm::vec3> berlinPoints = {
    glm::vec3(-350.0f, 0.0f, -120.0f),  // Station 0,
    glm::vec3(-340.0f, 0.0f, -120.0f),  // Station 1,
    glm::vec3(-330.0f, 0.0f, -120.0f),  // Station 2,
    glm::vec3(-320.0f, 0.0f, -120.0f),  // Station 3,
    glm::vec3(-310.0f, 0.0f, -120.0f),  // Station 4,
    glm::vec3(-300.0f, 0.0f, -120.0f),  // Station 5,
    glm::vec3(-290.0f, 0.0f, -120.0f),  // Station 6,
    glm::vec3(-280.0f, 0.0f, -120.0f),  // Station 7,
    glm::vec3(-270.0f, 0.0f, -120.0f),  // Station 8,
    glm::vec3(-260.0f, 0.0f, -120.0f),  // Station 9,
    glm::vec3(-250.0f, 0.0f, -120.0f),  // Station 10,
    glm::vec3(-240.0f, 0.0f, -120.0f),  // Station 11,
    glm::vec3(-230.0f, 0.0f, -120.0f),  // Station 12,
    glm::vec3(-220.0f, 0.0f, -120.0f),  // Station 13,
    glm::vec3(-210.0f, 0.0f, -120.0f),  // Station 14,
    glm::vec3(-200.0f, 0.0f, -120.0f),  // Station 15,
    glm::vec3(-190.0f, 0.0f, -120.0f),  // Station 16,
    glm::vec3(-180.0f, 0.0f, -120.0f),  // Station 17,
    glm::vec3(-170.0f, 0.0f, -120.0f),  // Station 18,
    glm::vec3(-160.0f, 0.0f, -120.0f),  // Station 19,
    glm::vec3(-150.0f, 0.0f, -80.0f),  // Station 20,
    glm::vec3(-140.0f, 0.0f, -80.0f),  // Station 21,
    glm::vec3(-130.0f, 0.0f, -80.0f),  // Station 22,
    glm::vec3(-120.0f, 0.0f, -80.0f),  // Station 23,
    glm::vec3(-110.0f, 0.0f, -80.0f),  // Station 24,
    glm::vec3(-100.0f, 0.0f, -80.0f),  // Station 25,
    glm::vec3(-90.0f, 0.0f, -80.0f),  // Station 26,
    glm::vec3(-80.0f, 0.0f, -80.0f),  // Station 27,
    glm::vec3(-70.0f, 0.0f, -80.0f),  // Station 28,
    glm::vec3(-60.0f, 0.0f, -80.0f),  // Station 29,
    glm::vec3(-50.0f, 0.0f, -80.0f),  // Station 30,
    glm::vec3(-40.0f, 0.0f, -80.0f),  // Station 31,
    glm::vec3(-30.0f, 0.0f, -80.0f),  // Station 32,
    glm::vec3(-20.0f, 0.0f, -80.0f),  // Station 33,
    glm::vec3(-10.0f, 0.0f, -80.0f),  // Station 34,
    glm::vec3(0.0f, 0.0f, -80.0f),  // Station 35,
    glm::vec3(10.0f, 0.0f, -80.0f),  // Station 36,
    glm::vec3(20.0f, 0.0f, -80.0f),  // Station 37,
    glm::vec3(30.0f, 0.0f, -80.0f),  // Station 38,
    glm::vec3(40.0f, 0.0f, -80.0f),  // Station 39,
    glm::vec3(50.0f, 0.0f, -40.0f),  // Station 40,
    glm::vec3(60.0f, 0.0f, -40.0f),  // Station 41,
    glm::vec3(70.0f, 0.0f, -40.0f),  // Station 42,
    glm::vec3(80.0f, 0.0f, -40.0f),  // Station 43,
    glm::vec3(90.0f, 0.0f, -40.0f),  // Station 44,
    glm::vec3(100.0f, 0.0f, -40.0f),  // Station 45,
    glm::vec3(110.0f, 0.0f, -40.0f),  // Station 46,
    glm::vec3(120.0f, 0.0f, -40.0f),  // Station 47,
    glm::vec3(130.0f, 0.0f, -40.0f),  // Station 48,
    glm::vec3(140.0f, 0.0f, -40.0f),  // Station 49,
    glm::vec3(150.0f, 0.0f, -40.0f),  // Station 50,
    glm::vec3(160.0f, 0.0f, -40.0f),  // Station 51,
    glm::vec3(170.0f, 0.0f, -40.0f),  // Station 52,
    glm::vec3(180.0f, 0.0f, -40.0f),  // Station 53,
    glm::vec3(190.0f, 0.0f, -40.0f),  // Station 54,
    glm::vec3(200.0f, 0.0f, -40.0f),  // Station 55,
    glm::vec3(210.0f, 0.0f, -40.0f),  // Station 56,
    glm::vec3(220.0f, 0.0f, -40.0f),  // Station 57,
    glm::vec3(230.0f, 0.0f, -40.0f),  // Station 58,
    glm::vec3(240.0f, 0.0f, -40.0f),  // Station 59,
    glm::vec3(250.0f, 0.0f, 0.0f),  // Station 60,
    glm::vec3(260.0f, 0.0f, 0.0f),  // Station 61,
    glm::vec3(270.0f, 0.0f, 0.0f),  // Station 62,
    glm::vec3(280.0f, 0.0f, 0.0f),  // Station 63,
    glm::vec3(290.0f, 0.0f, 0.0f),  // Station 64,
    glm::vec3(300.0f, 0.0f, 0.0f),  // Station 65,
    glm::vec3(310.0f, 0.0f, 0.0f),  // Station 66,
    glm::vec3(320.0f, 0.0f, 0.0f),  // Station 67,
    glm::vec3(330.0f, 0.0f, 0.0f),  // Station 68,
    glm::vec3(340.0f, 0.0f, 0.0f),  // Station 69,
    glm::vec3(-350.0f, 0.0f, 0.0f),  // Station 70,
    glm::vec3(-340.0f, 0.0f, 0.0f),  // Station 71,
    glm::vec3(-330.0f, 0.0f, 0.0f),  // Station 72,
    glm::vec3(-320.0f, 0.0f, 0.0f),  // Station 73,
    glm::vec3(-310.0f, 0.0f, 0.0f),  // Station 74,
    glm::vec3(-300.0f, 0.0f, 0.0f),  // Station 75,
    glm::vec3(-290.0f, 0.0f, 0.0f),  // Station 76,
    glm::vec3(-280.0f, 0.0f, 0.0f),  // Station 77,
    glm::vec3(-270.0f, 0.0f, 0.0f),  // Station 78,
    glm::vec3(-260.0f, 0.0f, 0.0f),  // Station 79,
    glm::vec3(-250.0f, 0.0f, 40.0f),  // Station 80,
    glm::vec3(-240.0f, 0.0f, 40.0f),  // Station 81,
    glm::vec3(-230.0f, 0.0f, 40.0f),  // Station 82,
    glm::vec3(-220.0f, 0.0f, 40.0f),  // Station 83,
    glm::vec3(-210.0f, 0.0f, 40.0f),  // Station 84,
    glm::vec3(-200.0f, 0.0f, 40.0f),  // Station 85,
    glm::vec3(-190.0f, 0.0f, 40.0f),  // Station 86,
    glm::vec3(-180.0f, 0.0f, 40.0f),  // Station 87,
    glm::vec3(-170.0f, 0.0f, 40.0f),  // Station 88,
    glm::vec3(-160.0f, 0.0f, 40.0f),  // Station 89,
    glm::vec3(-150.0f, 0.0f, 40.0f),  // Station 90,
    glm::vec3(-140.0f, 0.0f, 40.0f),  // Station 91,
    glm::vec3(-130.0f, 0.0f, 40.0f),  // Station 92,
    glm::vec3(-120.0f, 0.0f, 40.0f),  // Station 93,
    glm::vec3(-110.0f, 0.0f, 40.0f),  // Station 94,
    glm::vec3(-100.0f, 0.0f, 40.0f),  // Station 95,
    glm::vec3(-90.0f, 0.0f, 40.0f),  // Station 96,
    glm::vec3(-80.0f, 0.0f, 40.0f),  // Station 97,
    glm::vec3(-70.0f, 0.0f, 40.0f),  // Station 98,
    glm::vec3(-60.0f, 0.0f, 40.0f),  // Station 99,
    glm::vec3(-50.0f, 0.0f, 80.0f),  // Station 100,
    glm::vec3(-40.0f, 0.0f, 80.0f),  // Station 101,
    glm::vec3(-30.0f, 0.0f, 80.0f),  // Station 102,
    glm::vec3(-20.0f, 0.0f, 80.0f),  // Station 103,
    glm::vec3(-10.0f, 0.0f, 80.0f),  // Station 104,
    glm::vec3(0.0f, 0.0f, 80.0f),  // Station 105,
    glm::vec3(10.0f, 0.0f, 80.0f),  // Station 106,
    glm::vec3(20.0f, 0.0f, 80.0f),  // Station 107,
    glm::vec3(30.0f, 0.0f, 80.0f),  // Station 108,
    glm::vec3(40.0f, 0.0f, 80.0f),  // Station 109,
    glm::vec3(50.0f, 0.0f, 80.0f),  // Station 110,
    glm::vec3(60.0f, 0.0f, 80.0f),  // Station 111,
    glm::vec3(70.0f, 0.0f, 80.0f),  // Station 112,
    glm::vec3(80.0f, 0.0f, 80.0f),  // Station 113,
    glm::vec3(90.0f, 0.0f, 80.0f),  // Station 114,
    glm::vec3(100.0f, 0.0f, 80.0f),  // Station 115,
    glm::vec3(110.0f, 0.0f, 80.0f),  // Station 116,
    glm::vec3(120.0f, 0.0f, 80.0f),  // Station 117,
    glm::vec3(130.0f, 0.0f, 80.0f),  // Station 118,
    glm::vec3(140.0f, 0.0f, 80.0f),  // Station 119,
    glm::vec3(150.0f, 0.0f, 120.0f),  // Station 120,
    glm::vec3(160.0f, 0.0f, 120.0f),  // Station 121,
    glm::vec3(170.0f, 0.0f, 120.0f),  // Station 122,
    glm::vec3(180.0f, 0.0f, 120.0f),  // Station 123,
    glm::vec3(190.0f, 0.0f, 120.0f),  // Station 124,
    glm::vec3(200.0f, 0.0f, 120.0f),  // Station 125,
    glm::vec3(210.0f, 0.0f, 120.0f),  // Station 126,
    glm::vec3(220.0f, 0.0f, 120.0f),  // Station 127,
    glm::vec3(230.0f, 0.0f, 120.0f),  // Station 128,
    glm::vec3(240.0f, 0.0f, 120.0f),  // Station 129,
    glm::vec3(250.0f, 0.0f, 120.0f),  // Station 130,
    glm::vec3(260.0f, 0.0f, 120.0f),  // Station 131,
    glm::vec3(270.0f, 0.0f, 120.0f),  // Station 132,
    glm::vec3(280.0f, 0.0f, 120.0f),  // Station 133,
    glm::vec3(290.0f, 0.0f, 120.0f),  // Station 134,
    glm::vec3(300.0f, 0.0f, 120.0f),  // Station 135,
    glm::vec3(310.0f, 0.0f, 120.0f),  // Station 136,
    glm::vec3(320.0f, 0.0f, 120.0f),  // Station 137,
    glm::vec3(330.0f, 0.0f, 120.0f),  // Station 138,
    glm::vec3(340.0f, 0.0f, 120.0f),  // Station 139,
    glm::vec3(-350.0f, 0.0f, 160.0f),  // Station 140,
    glm::vec3(-340.0f, 0.0f, 160.0f),  // Station 141,
    glm::vec3(-330.0f, 0.0f, 160.0f),  // Station 142,
    glm::vec3(-320.0f, 0.0f, 160.0f),  // Station 143,
    glm::vec3(-310.0f, 0.0f, 160.0f),  // Station 144,
    glm::vec3(-300.0f, 0.0f, 160.0f),  // Station 145,
    glm::vec3(-290.0f, 0.0f, 160.0f),  // Station 146,
    glm::vec3(-280.0f, 0.0f, 160.0f),  // Station 147,
    glm::vec3(-270.0f, 0.0f, 160.0f),  // Station 148,
    glm::vec3(-260.0f, 0.0f, 160.0f),  // Station 149,
    glm::vec3(-250.0f, 0.0f, 160.0f),  // Station 150,
    glm::vec3(-240.0f, 0.0f, 160.0f),  // Station 151,
    glm::vec3(-230.0f, 0.0f, 160.0f),  // Station 152,
    glm::vec3(-220.0f, 0.0f, 160.0f),  // Station 153,
    glm::vec3(-210.0f, 0.0f, 160.0f),  // Station 154,
    glm::vec3(-200.0f, 0.0f, 160.0f),  // Station 155,
    glm::vec3(-190.0f, 0.0f, 160.0f),  // Station 156,
    glm::vec3(-180.0f, 0.0f, 160.0f),  // Station 157,
    glm::vec3(-170.0f, 0.0f, 160.0f),  // Station 158,
    glm::vec3(-160.0f, 0.0f, 160.0f),  // Station 159,
    glm::vec3(-150.0f, 0.0f, 200.0f),  // Station 160,
    glm::vec3(-140.0f, 0.0f, 200.0f),  // Station 161,
    glm::vec3(-130.0f, 0.0f, 200.0f),  // Station 162,
    glm::vec3(-120.0f, 0.0f, 200.0f),  // Station 163,
    glm::vec3(-110.0f, 0.0f, 200.0f),  // Station 164,
    glm::vec3(-100.0f, 0.0f, 200.0f),  // Station 165,
    glm::vec3(-90.0f, 0.0f, 200.0f),  // Station 166,
    glm::vec3(-80.0f, 0.0f, 200.0f),  // Station 167,
    glm::vec3(-70.0f, 0.0f, 200.0f),  // Station 168,
    glm::vec3(-60.0f, 0.0f, 200.0f),  // Station 169,
    glm::vec3(-50.0f, 0.0f, 200.0f),  // Station 170,
    glm::vec3(-40.0f, 0.0f, 200.0f),  // Station 171,
    glm::vec3(-30.0f, 0.0f, 200.0f),  // Station 172,
    glm::vec3(-20.0f, 0.0f, 200.0f),  // Station 173,
    glm::vec3(-10.0f, 0.0f, 200.0f),  // Station 174
};

std::vector<std::vector<int>> berlinRoutes = {
    {0, 9, 18, 27, 36, 45, 54, 63, 72, 81, 90, 99, 108, 117, 126, 135, 144, 153, 162, 171},
    {1, 10, 19, 28, 37, 46, 55, 64, 73, 82, 91, 100, 109, 118, 127, 136, 145, 154, 163, 172},
    {2, 11, 20, 29, 38, 47, 56, 65, 74, 83, 92, 101, 110, 119, 128, 137, 146, 155, 164, 173},
    {3, 12, 21, 30, 39, 48, 57, 66, 75, 84, 93, 102, 111, 120, 129, 138, 147, 156, 165, 174},
    {4, 13, 22, 31, 40, 49, 58, 67, 76, 85, 94, 103, 112, 121, 130, 139, 148, 157, 166},
    {5, 14, 23, 32, 41, 50, 59, 68, 77, 86, 95, 104, 113, 122, 131, 140, 149, 158, 167},
    {6, 15, 24, 33, 42, 51, 60, 69, 78, 87, 96, 105, 114, 123, 132, 141, 150, 159, 168},
    {7, 16, 25, 34, 43, 52, 61, 70, 79, 88, 97, 106, 115, 124, 133, 142, 151, 160, 169},
    {8, 17, 26, 35, 44, 53, 62, 71, 80, 89, 98, 107, 116, 125, 134, 143, 152, 161, 170}
};

std::vector<std::string> berlinStations = {
    "Station_0",
    "Station_1",
    "Station_2",
    "Station_3",
    "Station_4",
    "Station_5",
    "Station_6",
    "Station_7",
    "Station_8",
    "Station_9",
    "Station_10",
    "Station_11",
    "Station_12",
    "Station_13",
    "Station_14",
    "Station_15",
    "Station_16",
    "Station_17",
    "Station_18",
    "Station_19",
    "Station_20",
    "Station_21",
    "Station_22",
    "Station_23",
    "Station_24",
    "Station_25",
    "Station_26",
    "Station_27",
    "Station_28",
    "Station_29",
    "Station_30",
    "Station_31",
    "Station_32",
    "Station_33",
    "Station_34",
    "Station_35",
    "Station_36",
    "Station_37",
    "Station_38",
    "Station_39",
    "Station_40",
    "Station_41",
    "Station_42",
    "Station_43",
    "Station_44",
    "Station_45",
    "Station_46",
    "Station_47",
    "Station_48",
    "Station_49",
    "Station_50",
    "Station_51",
    "Station_52",
    "Station_53",
    "Station_54",
    "Station_55",
    "Station_56",
    "Station_57",
    "Station_58",
    "Station_59",
    "Station_60",
    "Station_61",
    "Station_62",
    "Station_63",
    "Station_64",
    "Station_65",
    "Station_66",
    "Station_67",
    "Station_68",
    "Station_69",
    "Station_70",
    "Station_71",
    "Station_72",
    "Station_73",
    "Station_74",
    "Station_75",
    "Station_76",
    "Station_77",
    "Station_78",
    "Station_79",
    "Station_80",
    "Station_81",
    "Station_82",
    "Station_83",
    "Station_84",
    "Station_85",
    "Station_86",
    "Station_87",
    "Station_88",
    "Station_89",
    "Station_90",
    "Station_91",
    "Station_92",
    "Station_93",
    "Station_94",
    "Station_95",
    "Station_96",
    "Station_97",
    "Station_98",
    "Station_99",
    "Station_100",
    "Station_101",
    "Station_102",
    "Station_103",
    "Station_104",
    "Station_105",
    "Station_106",
    "Station_107",
    "Station_108",
    "Station_109",
    "Station_110",
    "Station_111",
    "Station_112",
    "Station_113",
    "Station_114",
    "Station_115",
    "Station_116",
    "Station_117",
    "Station_118",
    "Station_119",
    "Station_120",
    "Station_121",
    "Station_122",
    "Station_123",
    "Station_124",
    "Station_125",
    "Station_126",
    "Station_127",
    "Station_128",
    "Station_129",
    "Station_130",
    "Station_131",
    "Station_132",
    "Station_133",
    "Station_134",
    "Station_135",
    "Station_136",
    "Station_137",
    "Station_138",
    "Station_139",
    "Station_140",
    "Station_141",
    "Station_142",
    "Station_143",
    "Station_144",
    "Station_145",
    "Station_146",
    "Station_147",
    "Station_148",
    "Station_149",
    "Station_150",
    "Station_151",
    "Station_152",
    "Station_153",
    "Station_154",
    "Station_155",
    "Station_156",
    "Station_157",
    "Station_158",
    "Station_159",
    "Station_160",
    "Station_161",
    "Station_162",
    "Station_163",
    "Station_164",
    "Station_165",
    "Station_166",
    "Station_167",
    "Station_168",
    "Station_169",
    "Station_170",
    "Station_171",
    "Station_172",
    "Station_173",
    "Station_174"
};



// Stacje metra w Mińsku (x,0,z) w przybliżonym układzie 700x700
std::vector<glm::vec3> minskPoints = {
    // Maskouskaja (L1)
    glm::vec3(-300.0f, 0.0f,  200.0f), // Uruchcha (0)
    glm::vec3(-240.0f, 0.0f,  170.0f), // Barysawski trakt
    glm::vec3(-180.0f, 0.0f,  140.0f), // Uschod
    glm::vec3(-120.0f, 0.0f,  110.0f), // Maskowskaja
    glm::vec3(-60.0f,  0.0f,   80.0f), // Park Chalyuskintsaw
    glm::vec3(0.0f,    0.0f,   50.0f), // Akademiya Navuk
    glm::vec3(60.0f,   0.0f,   20.0f), // Ploshcha Yakuba Kolasa
    glm::vec3(120.0f,  0.0f,  -10.0f), // Ploshcha Peramohi
    glm::vec3(180.0f,  0.0f,  -40.0f), // Kastrychnitskaya
    glm::vec3(240.0f,  0.0f,  -70.0f), // Ploshcha Lyenina
    glm::vec3(300.0f,  0.0f, -100.0f), // Instytut Kultury
    glm::vec3(360.0f,  0.0f, -130.0f), // Hrushawka
    glm::vec3(420.0f,  0.0f, -160.0f), // Mikhalova
    glm::vec3(480.0f,  0.0f, -190.0f), // Pyatrowshchyna
    glm::vec3(540.0f,  0.0f, -220.0f), // Malinawka

    // Awtazavodskaja (L2)
    glm::vec3(-300.0f, 0.0f,  -200.0f), // Kamyennaya Horka (15)
    glm::vec3(-240.0f, 0.0f,  -230.0f), // Kuntsawshchyna
    glm::vec3(-180.0f, 0.0f,  -260.0f), // Spartywnaya
    glm::vec3(-120.0f, 0.0f,  -290.0f), // Pushkinskaya
    glm::vec3(-60.0f,  0.0f,  -320.0f), // Maladzyozhnaya
    glm::vec3(0.0f,    0.0f,  -350.0f), // Frunzyenskaya
    glm::vec3(60.0f,   0.0f,  -380.0f), // Nyamiha
    glm::vec3(120.0f,  0.0f,  -410.0f), // Kupalawskaya
    glm::vec3(180.0f,  0.0f,  -440.0f), // Pyershamayskaya
    glm::vec3(240.0f,  0.0f,  -470.0f), // Pralyetarskaya
    glm::vec3(300.0f,  0.0f,  -500.0f), // Traktarny zavod
    glm::vec3(360.0f,  0.0f,  -530.0f), // Partyzanskaya
    glm::vec3(420.0f,  0.0f,  -560.0f), // Awtazavodskaya
    glm::vec3(480.0f,  0.0f,  -590.0f), // Mahilyowskaya

    // Zelenaluzhskaja (L3)
    glm::vec3(-100.0f, 0.0f,  0.0f),     // Jubiliejnaja ploscha (29)
    glm::vec3(-40.0f,  0.0f,  -30.0f),   // Ploshcha Frantsishka Bahushevicha
    glm::vec3(20.0f,   0.0f,  -60.0f),   // Vakzalnaya
    glm::vec3(80.0f,   0.0f,  -90.0f),   // Kavalskaja Slabada
    glm::vec3(140.0f,  0.0f, -120.0f),   // Aeradromnaya
    glm::vec3(200.0f,  0.0f, -150.0f),   // Nemarshanski Sad
    glm::vec3(260.0f,  0.0f, -180.0f)    // Slutski Hastsinets
};

std::vector<std::vector<int>> minskRoutes = {
    // L1 (0–14)
    { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14 },
    // L2 (15–28)
    {15,16,17,18,19,20,21,22,23,24,25,26,27,28 },
    // L3 (29–35)
    {29,30,31,32,33,34,35 }
};

std::vector<std::string> minskStations = {
    // L1
    "Uruchcha","Barysawski trakt","Uschod","Maskowskaja","Park Chalyuskintsaw",
    "Akademiya Navuk","Ploshcha Yakuba Kolasa","Ploshcha Peramohi",
    "Kastrychnitskaya","Ploshcha Lyenina","Instytut Kultury",
    "Hrushawka","Mikhalova","Pyatrowshchyna","Malinawka",
    // L2
    "Kamyennaya Horka","Kuntsawshchyna","Spartywnaya","Pushkinskaya",
    "Maladzyozhnaya","Frunzyenskaya","Nyamiha","Kupalawskaya",
    "Pyershamayskaya","Pralyetarskaya","Traktarny zavod","Partyzanskaya",
    "Awtazavodskaya","Mahilyowskaya",
    // L3
    "Jubiliejnaja ploscha","Ploshcha Frantsishka Bahushevicha",
    "Vakzalnaya","Kavalskaja Slabada","Aeradromnaya",
    "Nemarshanski Sad","Slutski Hastsinets"
};


// Paris Metro Map (simplified) - wygenerowane automatycznie
std::vector<glm::vec3> parisPoints = {
    glm::vec3(-350.0f, 0.0f, -150.0f),  // Station 0,
    glm::vec3(-343.0f, 0.0f, -150.0f),  // Station 1,
    glm::vec3(-336.0f, 0.0f, -150.0f),  // Station 2,
    glm::vec3(-329.0f, 0.0f, -150.0f),  // Station 3,
    glm::vec3(-322.0f, 0.0f, -150.0f),  // Station 4,
    glm::vec3(-315.0f, 0.0f, -150.0f),  // Station 5,
    glm::vec3(-308.0f, 0.0f, -150.0f),  // Station 6,
    glm::vec3(-301.0f, 0.0f, -150.0f),  // Station 7,
    glm::vec3(-294.0f, 0.0f, -150.0f),  // Station 8,
    glm::vec3(-287.0f, 0.0f, -150.0f),  // Station 9,
    glm::vec3(-280.0f, 0.0f, -150.0f),  // Station 10,
    glm::vec3(-273.0f, 0.0f, -150.0f),  // Station 11,
    glm::vec3(-266.0f, 0.0f, -150.0f),  // Station 12,
    glm::vec3(-259.0f, 0.0f, -150.0f),  // Station 13,
    glm::vec3(-252.0f, 0.0f, -100.0f),  // Station 14,
    glm::vec3(-245.0f, 0.0f, -100.0f),  // Station 15,
    glm::vec3(-238.0f, 0.0f, -100.0f),  // Station 16,
    glm::vec3(-231.0f, 0.0f, -100.0f),  // Station 17,
    glm::vec3(-224.0f, 0.0f, -100.0f),  // Station 18,
    glm::vec3(-217.0f, 0.0f, -100.0f),  // Station 19,
    glm::vec3(-210.0f, 0.0f, -100.0f),  // Station 20,
    glm::vec3(-203.0f, 0.0f, -100.0f),  // Station 21,
    glm::vec3(-196.0f, 0.0f, -100.0f),  // Station 22,
    glm::vec3(-189.0f, 0.0f, -100.0f),  // Station 23,
    glm::vec3(-182.0f, 0.0f, -100.0f),  // Station 24,
    glm::vec3(-175.0f, 0.0f, -100.0f),  // Station 25,
    glm::vec3(-168.0f, 0.0f, -100.0f),  // Station 26,
    glm::vec3(-161.0f, 0.0f, -100.0f),  // Station 27,
    glm::vec3(-154.0f, 0.0f, -50.0f),  // Station 28,
    glm::vec3(-147.0f, 0.0f, -50.0f),  // Station 29,
    glm::vec3(-140.0f, 0.0f, -50.0f),  // Station 30,
    glm::vec3(-133.0f, 0.0f, -50.0f),  // Station 31,
    glm::vec3(-126.0f, 0.0f, -50.0f),  // Station 32,
    glm::vec3(-119.0f, 0.0f, -50.0f),  // Station 33,
    glm::vec3(-112.0f, 0.0f, -50.0f),  // Station 34,
    glm::vec3(-105.0f, 0.0f, -50.0f),  // Station 35,
    glm::vec3(-98.0f, 0.0f, -50.0f),  // Station 36,
    glm::vec3(-91.0f, 0.0f, -50.0f),  // Station 37,
    glm::vec3(-84.0f, 0.0f, -50.0f),  // Station 38,
    glm::vec3(-77.0f, 0.0f, -50.0f),  // Station 39,
    glm::vec3(-70.0f, 0.0f, -50.0f),  // Station 40,
    glm::vec3(-63.0f, 0.0f, -50.0f),  // Station 41,
    glm::vec3(-56.0f, 0.0f, 0.0f),  // Station 42,
    glm::vec3(-49.0f, 0.0f, 0.0f),  // Station 43,
    glm::vec3(-42.0f, 0.0f, 0.0f),  // Station 44,
    glm::vec3(-35.0f, 0.0f, 0.0f),  // Station 45,
    glm::vec3(-28.0f, 0.0f, 0.0f),  // Station 46,
    glm::vec3(-21.0f, 0.0f, 0.0f),  // Station 47,
    glm::vec3(-14.0f, 0.0f, 0.0f),  // Station 48,
    glm::vec3(-7.0f, 0.0f, 0.0f),  // Station 49,
    glm::vec3(0.0f, 0.0f, 0.0f),  // Station 50,
    glm::vec3(7.0f, 0.0f, 0.0f),  // Station 51,
    glm::vec3(14.0f, 0.0f, 0.0f),  // Station 52,
    glm::vec3(21.0f, 0.0f, 0.0f),  // Station 53,
    glm::vec3(28.0f, 0.0f, 0.0f),  // Station 54,
    glm::vec3(35.0f, 0.0f, 0.0f),  // Station 55,
    glm::vec3(42.0f, 0.0f, 50.0f),  // Station 56,
    glm::vec3(49.0f, 0.0f, 50.0f),  // Station 57,
    glm::vec3(56.0f, 0.0f, 50.0f),  // Station 58,
    glm::vec3(63.0f, 0.0f, 50.0f),  // Station 59,
    glm::vec3(70.0f, 0.0f, 50.0f),  // Station 60,
    glm::vec3(77.0f, 0.0f, 50.0f),  // Station 61,
    glm::vec3(84.0f, 0.0f, 50.0f),  // Station 62,
    glm::vec3(91.0f, 0.0f, 50.0f),  // Station 63,
    glm::vec3(98.0f, 0.0f, 50.0f),  // Station 64,
    glm::vec3(105.0f, 0.0f, 50.0f),  // Station 65,
    glm::vec3(112.0f, 0.0f, 50.0f),  // Station 66,
    glm::vec3(119.0f, 0.0f, 50.0f),  // Station 67,
    glm::vec3(126.0f, 0.0f, 50.0f),  // Station 68,
    glm::vec3(133.0f, 0.0f, 50.0f),  // Station 69,
    glm::vec3(140.0f, 0.0f, 100.0f),  // Station 70,
    glm::vec3(147.0f, 0.0f, 100.0f),  // Station 71,
    glm::vec3(154.0f, 0.0f, 100.0f),  // Station 72,
    glm::vec3(161.0f, 0.0f, 100.0f),  // Station 73,
    glm::vec3(168.0f, 0.0f, 100.0f),  // Station 74,
    glm::vec3(175.0f, 0.0f, 100.0f),  // Station 75,
    glm::vec3(182.0f, 0.0f, 100.0f),  // Station 76,
    glm::vec3(189.0f, 0.0f, 100.0f),  // Station 77,
    glm::vec3(196.0f, 0.0f, 100.0f),  // Station 78,
    glm::vec3(203.0f, 0.0f, 100.0f),  // Station 79,
    glm::vec3(210.0f, 0.0f, 100.0f),  // Station 80,
    glm::vec3(217.0f, 0.0f, 100.0f),  // Station 81,
    glm::vec3(224.0f, 0.0f, 100.0f),  // Station 82,
    glm::vec3(231.0f, 0.0f, 100.0f),  // Station 83,
    glm::vec3(238.0f, 0.0f, 150.0f),  // Station 84,
    glm::vec3(245.0f, 0.0f, 150.0f),  // Station 85,
    glm::vec3(252.0f, 0.0f, 150.0f),  // Station 86,
    glm::vec3(259.0f, 0.0f, 150.0f),  // Station 87,
    glm::vec3(266.0f, 0.0f, 150.0f),  // Station 88,
    glm::vec3(273.0f, 0.0f, 150.0f),  // Station 89,
    glm::vec3(280.0f, 0.0f, 150.0f),  // Station 90,
    glm::vec3(287.0f, 0.0f, 150.0f),  // Station 91,
    glm::vec3(294.0f, 0.0f, 150.0f),  // Station 92,
    glm::vec3(301.0f, 0.0f, 150.0f),  // Station 93,
    glm::vec3(308.0f, 0.0f, 150.0f),  // Station 94,
    glm::vec3(315.0f, 0.0f, 150.0f),  // Station 95,
    glm::vec3(322.0f, 0.0f, 150.0f),  // Station 96,
    glm::vec3(329.0f, 0.0f, 150.0f),  // Station 97,
    glm::vec3(336.0f, 0.0f, 200.0f),  // Station 98,
    glm::vec3(343.0f, 0.0f, 200.0f),  // Station 99
};

std::vector<std::vector<int>> parisRoutes = {
    {0, 14, 28, 42, 56, 70, 84, 98},
    {1, 15, 29, 43, 57, 71, 85, 99},
    {2, 16, 30, 44, 58, 72, 86},
    {3, 17, 31, 45, 59, 73, 87},
    {4, 18, 32, 46, 60, 74, 88},
    {5, 19, 33, 47, 61, 75, 89},
    {6, 20, 34, 48, 62, 76, 90},
    {7, 21, 35, 49, 63, 77, 91},
    {8, 22, 36, 50, 64, 78, 92},
    {9, 23, 37, 51, 65, 79, 93},
    {10, 24, 38, 52, 66, 80, 94},
    {11, 25, 39, 53, 67, 81, 95},
    {12, 26, 40, 54, 68, 82, 96},
    {13, 27, 41, 55, 69, 83, 97}
};

std::vector<std::string> parisStations = {
    "Station_0",
    "Station_1",
    "Station_2",
    "Station_3",
    "Station_4",
    "Station_5",
    "Station_6",
    "Station_7",
    "Station_8",
    "Station_9",
    "Station_10",
    "Station_11",
    "Station_12",
    "Station_13",
    "Station_14",
    "Station_15",
    "Station_16",
    "Station_17",
    "Station_18",
    "Station_19",
    "Station_20",
    "Station_21",
    "Station_22",
    "Station_23",
    "Station_24",
    "Station_25",
    "Station_26",
    "Station_27",
    "Station_28",
    "Station_29",
    "Station_30",
    "Station_31",
    "Station_32",
    "Station_33",
    "Station_34",
    "Station_35",
    "Station_36",
    "Station_37",
    "Station_38",
    "Station_39",
    "Station_40",
    "Station_41",
    "Station_42",
    "Station_43",
    "Station_44",
    "Station_45",
    "Station_46",
    "Station_47",
    "Station_48",
    "Station_49",
    "Station_50",
    "Station_51",
    "Station_52",
    "Station_53",
    "Station_54",
    "Station_55",
    "Station_56",
    "Station_57",
    "Station_58",
    "Station_59",
    "Station_60",
    "Station_61",
    "Station_62",
    "Station_63",
    "Station_64",
    "Station_65",
    "Station_66",
    "Station_67",
    "Station_68",
    "Station_69",
    "Station_70",
    "Station_71",
    "Station_72",
    "Station_73",
    "Station_74",
    "Station_75",
    "Station_76",
    "Station_77",
    "Station_78",
    "Station_79",
    "Station_80",
    "Station_81",
    "Station_82",
    "Station_83",
    "Station_84",
    "Station_85",
    "Station_86",
    "Station_87",
    "Station_88",
    "Station_89",
    "Station_90",
    "Station_91",
    "Station_92",
    "Station_93",
    "Station_94",
    "Station_95",
    "Station_96",
    "Station_97",
    "Station_98",
    "Station_99"
};
#endif //CITY_INFO_H
