#include "pch.h"
#include "Error.h"
namespace Error
{
	// 0-99 системные ошибки
	// 100-109 ошибки параметров
	// 110-119 ошибки открытия файлов
	ERROR errors[ERROR_MAX_ENTRY]
	{
		ERROR_ENTRY(0,"Недопустимый код ошибки"),
		ERROR_ENTRY(1,"Системный сбой"),
		ERROR_ENTRY(2,"Вы пытаетесь превысить размер таблицы лексем"),ERROR_ENTRY(3,"Вы пытаетесь превысить размер таблицы идентификаторов"),ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY(5,"Не найдена парная скобка"),ERROR_ENTRY(6,"Найдено более одной функции main"),ERROR_ENTRY(7, "Превышена длина строкового литерала"),
		ERROR_ENTRY(8,"Отсутствует main"),ERROR_ENTRY(9,"Нераспознанная лексема"),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20),
		ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60),
		ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80),
		ERROR_ENTRY_NODEF10(90), ERROR_ENTRY(100,"Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104,"Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105),ERROR_ENTRY_NODEF(106),ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110,"Ошибка при открытии файла с иходным кодом (-in)"),
		ERROR_ENTRY(111,"Недопустимый символ в исходном коде (-in)"),
		ERROR_ENTRY(112,"Ошибка при создании файла (-log)"),
		ERROR_ENTRY(113,"Недопустимый символ в исходном коде"), ERROR_ENTRY(114, "Ошибка открытия out файла"),
		ERROR_ENTRY(115,"Ошибка открытия файла, содержащего байт-код"),  ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117),  ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130),
		ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170),
		ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"Неверная структура программы"),
		ERROR_ENTRY(601,"Ошибочный оператор"),
		ERROR_ENTRY(602,"Ошибка в выражении"),
		ERROR_ENTRY(603, "Ошибка в параметрах функции"),
		ERROR_ENTRY(604,"Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(605, "Ошибка мат. оператора"), ERROR_ENTRY(606,"Ошибка в параметрах функции cmp[str parm1, str parm2]"),
		ERROR_ENTRY(607,"Ошибка в параметрах функции clength[str parm]"), ERROR_ENTRY(608, "Ошибка в операторе if... then ... else ..."),
		ERROR_ENTRY(609,"Ошибка в объявлении идентификатора"), ERROR_ENTRY(610, "Ошибка в теле процедуры"),
		ERROR_ENTRY(611,"Ошибка в теле main"),ERROR_ENTRY(611, ""),ERROR_ENTRY_NODEF(613), ERROR_ENTRY_NODEF(614), ERROR_ENTRY_NODEF(615), ERROR_ENTRY_NODEF(616),
		ERROR_ENTRY_NODEF(617), ERROR_ENTRY_NODEF(618), ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630),
		ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY(700,"Использование необъявленного идентификатора"), ERROR_ENTRY(701, "Несоответствие парметров при вызове и при объявлении функции"),
		ERROR_ENTRY(702,"Повторное объявление функции или процедуры"), ERROR_ENTRY(703,"Повторное объявление процедуры"),
		ERROR_ENTRY(704,"Повторное объявление Переменной"), ERROR_ENTRY(705, "Несоответствие типов левой и правой части выражения"),
		ERROR_ENTRY(706,"Не соответствие типов(Операторы >, < возвращают boolean значение) "), ERROR_ENTRY(707, "Не найдено объявление функции"),
		ERROR_ENTRY(708, "Не найдено объявление процедуры"), ERROR_ENTRY(709, "Тип левой части выражения не определен"),
		ERROR_ENTRY(710, "Недопустимо использование мат. операций над строками"), ERROR_ENTRY(711, "Недопустимый вызов процедуры"),
		ERROR_ENTRY(712,"Несоответствие типа возвращаемого значения функции и выражения return"), ERROR_ENTRY(713, "Ошибка в условии оператора if"), ERROR_ENTRY(714, "Ошибка в операторе print"), ERROR_ENTRY_NODEF(715),
		ERROR_ENTRY_NODEF(716), ERROR_ENTRY_NODEF(717), ERROR_ENTRY_NODEF(718), ERROR_ENTRY_NODEF(719),
		ERROR_ENTRY_NODEF10(720),
		ERROR_ENTRY_NODEF10(730), 	ERROR_ENTRY_NODEF10(740),
		ERROR_ENTRY_NODEF10(750),	ERROR_ENTRY_NODEF10(760),
		ERROR_ENTRY_NODEF10(770),	ERROR_ENTRY_NODEF10(780),
		ERROR_ENTRY_NODEF10(790),	
		ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
		ERROR geterror(int id)
		{
			if (0<id && id < ERROR_MAX_ENTRY)
			{
				return errors[id];
			}
			else return errors[0];

		}
		ERROR geterrorin(int id, int line, int col)
		{
			if (0 < id && id < ERROR_MAX_ENTRY)
			{
				errors[id].inext.line = line;
				errors[id].inext.col = col;
				return errors[id];
			}
			else return errors[0];
		}
		ERROR getSemanticError(int id, int line, std::string str)
		{
			if (0 < id && id < ERROR_MAX_ENTRY)
			{
				errors[id].inext.line = line;
				str = ": " + str;
				strcat_s(errors[id].message, str.c_str());
				return errors[id];
			}
			else return errors[0];
		}
};