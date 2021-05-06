#ifndef K_DESCRIPTION_H
#define K_DESCRIPTION_H



enum KDescription {
    Umarsh_limit = 11, //f Ограничение максимального сигнала СУ по маршу
    Upsi_limit, //f Ограничение максимального сигнала СУ по курсу
    Uteta_limit, //f Ограничение максимального сигнала СУ по дифференту
    Ugamma_limit, //f Ограничение максимального сигнала СУ по крену

    Umvl_limit = 23, //f Ограничение максимального напряжения на ВМА МВЛ
    Umnl_limit, //f Ограничение максимального напряжения на ВМА МНЛ
    Umvp_limit, //f Ограничение максимального напряжения на ВМА МВП
    Umnp_limit, //f Ограничение максимального напряжения на ВМА МНП

    Kurs_ruchnoi_scale = 40,
    Kurs_otladka = 43,
    Kurs_K1,
    Kurs_K2,
    Limit_Upsi = 49,

};

#endif // K_DESCRIPTION_H
