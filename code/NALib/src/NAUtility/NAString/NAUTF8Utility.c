
#include "../NAString.h"

// Source: https://pastebin.com/fuw4Uizk

#define NA_U8_UPPER_LOWER_COUNT 1456

const NAUTF8Char* na_u8Uppers[NA_U8_UPPER_LOWER_COUNT] = {
  // 0
  u8"Ѐ", u8"Ё", u8"Ђ", u8"Ѓ", u8"Є", u8"Ѕ", u8"І", u8"Ї", u8"Ј", u8"Љ",
  u8"Њ", u8"Ћ", u8"Ќ", u8"Ѝ", u8"Ў", u8"Џ", u8"А", u8"Б", u8"В", u8"Г",
  u8"Д", u8"Е", u8"Ж", u8"З", u8"И", u8"Й", u8"К", u8"Л", u8"М", u8"Н",
  u8"О", u8"П", u8"Р", u8"С", u8"Т", u8"У", u8"Ф", u8"Х", u8"Ц", u8"Ч",
  u8"Ш", u8"Щ", u8"Ъ", u8"Ы", u8"Ь", u8"Э", u8"Ю", u8"Я", u8"Ѡ", u8"Ѣ",
  u8"Ѥ", u8"Ѧ", u8"Ѩ", u8"Ѫ", u8"Ѭ", u8"Ѯ", u8"Ѱ", u8"Ѳ", u8"Ѵ", u8"Ѷ",
  u8"Ѹ", u8"Ѻ", u8"Ѽ", u8"Ѿ", u8"Ҁ", u8"Ҋ", u8"Ҍ", u8"Ҏ", u8"Ґ", u8"Ғ",
  u8"Ҕ", u8"Җ", u8"Ҙ", u8"Қ", u8"Ҝ", u8"Ҟ", u8"Ҡ", u8"Ң", u8"Ҥ", u8"Ҧ",
  u8"Ҩ", u8"Ҫ", u8"Ҭ", u8"Ү", u8"Ұ", u8"Ҳ", u8"Ҵ", u8"Ҷ", u8"Ҹ", u8"Һ",
  u8"Ҽ", u8"Ҿ", u8"Ӂ", u8"Ӄ", u8"Ӆ", u8"Ӈ", u8"Ӊ", u8"Ӌ", u8"Ӎ", u8"Ӑ",
  // 100
  u8"Ӓ", u8"Ӕ", u8"Ӗ", u8"Ә", u8"Ӛ", u8"Ӝ", u8"Ӟ", u8"Ӡ", u8"Ӣ", u8"Ӥ",
  u8"Ӧ", u8"Ө", u8"Ӫ", u8"Ӭ", u8"Ӯ", u8"Ӱ", u8"Ӳ", u8"Ӵ", u8"Ӷ", u8"Ӹ",
  u8"Ӻ", u8"Ӽ", u8"Ӿ", u8"Ԁ", u8"Ԃ", u8"Ԅ", u8"Ԇ", u8"Ԉ", u8"Ԋ", u8"Ԍ",
  u8"Ԏ", u8"Ԑ", u8"Ԓ", u8"Ԕ", u8"Ԗ", u8"Ԙ", u8"Ԛ", u8"Ԝ", u8"Ԟ", u8"Ԡ",
  u8"Ԣ", u8"Ԥ", u8"Ԧ", u8"Ա", u8"Բ", u8"Գ", u8"Դ", u8"Ե", u8"Զ", u8"Է",
  u8"Ը", u8"Թ", u8"Ժ", u8"Ի", u8"Լ", u8"Խ", u8"Ծ", u8"Կ", u8"Հ", u8"Ձ",
  u8"Ղ", u8"Ճ", u8"Մ", u8"Յ", u8"Ն", u8"Շ", u8"Ո", u8"Չ", u8"Պ", u8"Ջ",
  u8"Ռ", u8"Ս", u8"Վ", u8"Տ", u8"Ր", u8"Ց", u8"Ւ", u8"Փ", u8"Ք", u8"Օ",
  u8"Ֆ", u8"A", u8"B", u8"C", u8"D", u8"E", u8"F", u8"G", u8"H", u8"I",
  u8"J", u8"K", u8"L", u8"M", u8"N", u8"O", u8"P", u8"Q", u8"R", u8"S",
  // 200
  u8"T", u8"U", u8"V", u8"W", u8"X", u8"Y", u8"Z", u8"À", u8"Á", u8"Â",
  u8"Ã", u8"Ä", u8"Å", u8"Æ", u8"Ç", u8"È", u8"É", u8"Ê", u8"Ë", u8"Ì",
  u8"Í", u8"Î", u8"Ï", u8"Ð", u8"Ñ", u8"Ò", u8"Ó", u8"Ô", u8"Õ", u8"Ö",
  u8"Ø", u8"Ù", u8"Ú", u8"Û", u8"Ü", u8"Ý", u8"Þ", u8"Ā", u8"Ă", u8"Ą",
  u8"Ć", u8"Ĉ", u8"Ċ", u8"Č", u8"Ď", u8"Đ", u8"Ē", u8"Ĕ", u8"Ė", u8"Ę",
  u8"Ě", u8"Ĝ", u8"Ğ", u8"Ġ", u8"Ģ", u8"Ĥ", u8"Ħ", u8"Ĩ", u8"Ī", u8"Ĭ",
  u8"Į", u8"Ĳ", u8"Ĵ", u8"Ķ", u8"Ĺ", u8"Ļ", u8"Ľ", u8"Ŀ", u8"Ł", u8"Ń",
  u8"Ņ", u8"Ň", u8"Ŋ", u8"Ō", u8"Ŏ", u8"Ő", u8"Œ", u8"Ŕ", u8"Ŗ", u8"Ř",
  u8"Ś", u8"Ŝ", u8"Ş", u8"Š", u8"Ţ", u8"Ť", u8"Ŧ", u8"Ũ", u8"Ū", u8"Ŭ",
  u8"Ů", u8"Ű", u8"Ų", u8"Ŵ", u8"Ŷ", u8"Ÿ", u8"Ź", u8"Ż", u8"Ž", u8"Ɓ",
  // 300
  u8"Ƃ", u8"Ƅ", u8"Ɔ", u8"Ƈ", u8"Ɗ", u8"Ƌ", u8"Ǝ", u8"Ə", u8"Ɛ", u8"Ƒ",
  u8"Ɠ", u8"Ɣ", u8"Ɩ", u8"Ɨ", u8"Ƙ", u8"Ɯ", u8"Ɲ", u8"Ơ", u8"Ƣ", u8"Ƥ",
  u8"Ƨ", u8"Ʃ", u8"Ƭ", u8"Ʈ", u8"Ư", u8"Ʊ", u8"Ʋ", u8"Ƴ", u8"Ƶ", u8"Ʒ",
  u8"Ƹ", u8"Ƽ", u8"Ǆ", u8"Ǉ", u8"Ǌ", u8"Ǎ", u8"Ǐ", u8"Ǒ", u8"Ǔ", u8"Ǖ",
  u8"Ǘ", u8"Ǚ", u8"Ǜ", u8"Ǟ", u8"Ǡ", u8"Ǣ", u8"Ǥ", u8"Ǧ", u8"Ǩ", u8"Ǫ",
  u8"Ǭ", u8"Ǯ", u8"Ǳ", u8"Ǵ", u8"Ǹ", u8"Ǻ", u8"Ǽ", u8"Ǿ", u8"Ȁ", u8"Ȃ",
  u8"Ȅ", u8"Ȇ", u8"Ȉ", u8"Ȋ", u8"Ȍ", u8"Ȏ", u8"Ȑ", u8"Ȓ", u8"Ȕ", u8"Ȗ",
  u8"Ș", u8"Ț", u8"Ȝ", u8"Ȟ", u8"Ƞ", u8"Ȣ", u8"Ȥ", u8"Ȧ", u8"Ȩ", u8"Ȫ",
  u8"Ȭ", u8"Ȯ", u8"Ȱ", u8"Ȳ", u8"Ⱥ", u8"Ȼ", u8"Ƚ", u8"Ⱦ", u8"Ɂ", u8"Ƀ",
  u8"Ʉ", u8"Ʌ", u8"Ɇ", u8"Ɉ", u8"Ɍ", u8"Ɏ", u8"Ͱ", u8"Ͳ", u8"Ͷ", u8"Ά",
  // 400
  u8"Έ", u8"Ή", u8"Ί", u8"Ό", u8"Ύ", u8"Ώ", u8"Α", u8"Β", u8"Γ", u8"Δ",
  u8"Ε", u8"Ζ", u8"Η", u8"Θ", u8"Ι", u8"Κ", u8"Λ", u8"Μ", u8"Ν", u8"Ξ",
  u8"Ο", u8"Π", u8"Ρ", u8"Σ", u8"Τ", u8"Υ", u8"Φ", u8"Χ", u8"Ψ", u8"Ω",
  u8"Ϊ", u8"Ϋ", u8"Ϣ", u8"Ϥ", u8"Ϧ", u8"Ϩ", u8"Ϫ", u8"Ϭ", u8"Ϯ", u8"Ϸ",
  u8"Ϻ", u8"Ͻ", u8"Ͼ", u8"Ͽ", u8"Ⴀ", u8"Ⴁ", u8"Ⴂ", u8"Ⴃ", u8"Ⴄ", u8"Ⴅ",
  u8"Ⴆ", u8"Ⴇ", u8"Ⴈ", u8"Ⴉ", u8"Ⴊ", u8"Ⴋ", u8"Ⴌ", u8"Ⴍ", u8"Ⴎ", u8"Ⴏ",
  u8"Ⴐ", u8"Ⴑ", u8"Ⴒ", u8"Ⴓ", u8"Ⴔ", u8"Ⴕ", u8"Ⴖ", u8"Ⴗ", u8"Ⴘ", u8"Ⴙ",
  u8"Ⴚ", u8"Ⴛ", u8"Ⴜ", u8"Ⴝ", u8"Ⴞ", u8"Ⴟ", u8"Ⴠ", u8"Ⴡ", u8"Ⴢ", u8"Ⴣ",
  u8"Ⴤ", u8"Ⴥ", u8"🔠", u8"Ａ", u8"Ｂ", u8"Ｃ", u8"Ｄ", u8"Ｅ", u8"Ｆ", u8"Ｇ",
  u8"Ｈ", u8"Ｉ", u8"Ｊ", u8"Ｋ", u8"Ｌ", u8"Ｍ", u8"Ｎ", u8"Ｏ", u8"Ｐ", u8"Ｑ",
  // 500
  u8"Ｒ", u8"Ｓ", u8"Ｔ", u8"Ｕ", u8"Ｖ", u8"Ｗ", u8"Ｘ", u8"Ｙ", u8"Ｚ", u8"Ⓐ",
  u8"Ⓑ", u8"Ⓒ", u8"Ⓓ", u8"Ⓔ", u8"Ⓕ", u8"Ⓖ", u8"Ⓗ", u8"Ⓘ", u8"Ⓙ", u8"Ⓚ",
  u8"Ⓛ", u8"Ⓜ", u8"Ⓝ", u8"Ⓞ", u8"Ⓟ", u8"Ⓠ", u8"Ⓡ", u8"Ⓢ", u8"Ⓣ", u8"Ⓤ",
  u8"Ⓥ", u8"Ⓦ", u8"Ⓧ", u8"Ⓨ", u8"Ⓩ", u8"Ⰰ", u8"Ⰱ", u8"Ⰲ", u8"Ⰳ", u8"Ⰴ",
  u8"Ⰵ", u8"Ⰶ", u8"Ⰷ", u8"Ⰸ", u8"Ⰹ", u8"Ⰺ", u8"Ⰻ", u8"Ⰼ", u8"Ⰽ", u8"Ⰾ",
  u8"Ⰿ", u8"Ⱀ", u8"Ⱁ", u8"Ⱂ", u8"Ⱃ", u8"Ⱄ", u8"Ⱅ", u8"Ⱆ", u8"Ⱇ", u8"Ⱈ",
  u8"Ⱉ", u8"Ⱊ", u8"Ⱋ", u8"Ⱌ", u8"Ⱍ", u8"Ⱎ", u8"Ⱏ", u8"Ⱐ", u8"Ⱑ", u8"Ⱒ",
  u8"Ⱓ", u8"Ⱔ", u8"Ⱕ", u8"Ⱖ", u8"Ⱗ", u8"Ⱘ", u8"Ⱙ", u8"Ⱚ", u8"Ⱛ", u8"Ⱜ",
  u8"Ⱝ", u8"Ⱞ", u8"Ⱡ", u8"Ɫ", u8"Ᵽ", u8"Ɽ", u8"Ⱨ", u8"Ⱪ", u8"Ⱬ", u8"Ɑ",
  u8"Ɱ", u8"Ɐ", u8"Ɒ", u8"Ⱳ", u8"Ⱶ", u8"ⱽ", u8"Ȿ", u8"Ɀ", u8"Ⲁ", u8"Ⲃ",
  // 600
  u8"Ⲅ", u8"Ⲇ", u8"Ⲉ", u8"Ⲋ", u8"Ⲍ", u8"Ⲏ", u8"Ⲑ", u8"Ⲓ", u8"Ⲕ", u8"Ⲗ",
  u8"Ⲙ", u8"Ⲛ", u8"Ⲝ", u8"Ⲟ", u8"Ⲡ", u8"Ⲣ", u8"Ⲥ", u8"Ⲧ", u8"Ⲩ", u8"Ⲫ",
  u8"Ⲭ", u8"Ⲯ", u8"Ⲱ", u8"Ⲳ", u8"Ⲵ", u8"Ⲷ", u8"Ⲹ", u8"Ⲻ", u8"Ⲽ", u8"Ⲿ",
  u8"Ⳁ", u8"Ⳃ", u8"Ⳅ", u8"Ⳇ", u8"Ⳉ", u8"Ⳋ", u8"Ⳍ", u8"Ⳏ", u8"Ⳑ", u8"Ⳓ",
  u8"Ⳕ", u8"Ⳗ", u8"Ⳙ", u8"Ⳛ", u8"Ⳝ", u8"Ⳟ", u8"Ⳡ", u8"Ⳣ", u8"Ⳬ", u8"Ⳮ",
  u8"𝐀", u8"𝐁", u8"𝐂", u8"𝐃", u8"𝐄", u8"𝐅", u8"𝐆", u8"𝐇", u8"𝐈", u8"𝐉",
  u8"𝐊", u8"𝐋", u8"𝐌", u8"𝐍", u8"𝐎", u8"𝐏", u8"𝐐", u8"𝐑", u8"𝐒", u8"𝐓",
  u8"𝐔", u8"𝐕", u8"𝐖", u8"𝐗", u8"𝐘", u8"𝐙", u8"𝐴", u8"𝐵", u8"𝐶", u8"𝐷",
  u8"𝐸", u8"𝐹", u8"𝐺", u8"𝐼", u8"𝐽", u8"𝐾", u8"𝐿", u8"𝑀", u8"𝑁", u8"𝑂",
  u8"𝑃", u8"𝑄", u8"𝑅", u8"𝑆", u8"𝑇", u8"𝑈", u8"𝑉", u8"𝑊", u8"𝑋", u8"𝑌",
  // 700
  u8"𝑍", u8"𝑨", u8"𝑩", u8"𝑪", u8"𝑫", u8"𝑬", u8"𝑭", u8"𝑮", u8"𝑯", u8"𝑰",
  u8"𝑱", u8"𝑲", u8"𝑳", u8"𝑴", u8"𝑵", u8"𝑶", u8"𝑷", u8"𝑸", u8"𝑹", u8"𝑺",
  u8"𝑻", u8"𝑼", u8"𝑽", u8"𝑾", u8"𝑿", u8"𝒀", u8"𝒁", u8"𝒜", u8"𝒞", u8"𝒟",
  u8"𝒥", u8"𝒦", u8"𝒩", u8"𝒫", u8"𝒬", u8"𝒮", u8"𝒯", u8"𝒰", u8"𝒱", u8"𝒲",
  u8"𝒳", u8"𝒴", u8"𝒵", u8"𝓐", u8"𝓑", u8"𝓒", u8"𝓓", u8"𝓔", u8"𝓕", u8"𝓖",
  u8"𝓗", u8"𝓘", u8"𝓙", u8"𝓚", u8"𝓛", u8"𝓜", u8"𝓝", u8"𝓞", u8"𝓟", u8"𝓠",
  u8"𝓡", u8"𝓢", u8"𝓣", u8"𝓤", u8"𝓥", u8"𝓦", u8"𝓧", u8"𝓨", u8"𝓩", u8"𝔄",
  u8"𝔅", u8"𝔇", u8"𝔈", u8"𝔉", u8"𝔊", u8"𝔍", u8"𝔎", u8"𝔏", u8"𝔐", u8"𝔑",
  u8"𝔒", u8"𝔓", u8"𝔔", u8"𝔖", u8"𝔗", u8"𝔘", u8"𝔙", u8"𝔚", u8"𝔛", u8"𝔜",
  u8"𝔸", u8"𝔹", u8"𝔻", u8"𝔼", u8"𝔽", u8"𝔾", u8"𝕀", u8"𝕁", u8"𝕂", u8"𝕃",
  // 800
  u8"𝕄", u8"𝕆", u8"𝕊", u8"𝕋", u8"𝕌", u8"𝕍", u8"𝕎", u8"𝕏", u8"𝕐", u8"𝕬",
  u8"𝕭", u8"𝕮", u8"𝕯", u8"𝕰", u8"𝕱", u8"𝕲", u8"𝕳", u8"𝕴", u8"𝕵", u8"𝕶",
  u8"𝕷", u8"𝕸", u8"𝕹", u8"𝕺", u8"𝕻", u8"𝕼", u8"𝕽", u8"𝕾", u8"𝕿", u8"𝖀",
  u8"𝖁", u8"𝖂", u8"𝖃", u8"𝖄", u8"𝖅", u8"𝖠", u8"𝖡", u8"𝖢", u8"𝖣", u8"𝖤",
  u8"𝖥", u8"𝖦", u8"𝖧", u8"𝖨", u8"𝖩", u8"𝖪", u8"𝖫", u8"𝖬", u8"𝖭", u8"𝖮",
  u8"𝖯", u8"𝖰", u8"𝖱", u8"𝖲", u8"𝖳", u8"𝖴", u8"𝖵", u8"𝖶", u8"𝖷", u8"𝖸",
  u8"𝖹", u8"𝗔", u8"𝗕", u8"𝗖", u8"𝗗", u8"𝗘", u8"𝗙", u8"𝗚", u8"𝗛", u8"𝗜",
  u8"𝗝", u8"𝗞", u8"𝗟", u8"𝗠", u8"𝗡", u8"𝗢", u8"𝗣", u8"𝗤", u8"𝗥", u8"𝗦",
  u8"𝗧", u8"𝗨", u8"𝗩", u8"𝗪", u8"𝗫", u8"𝗬", u8"𝗭", u8"𝘈", u8"𝘉", u8"𝘊",
  u8"𝘋", u8"𝘌", u8"𝘍", u8"𝘎", u8"𝘏", u8"𝘐", u8"𝘑", u8"𝘒", u8"𝘓", u8"𝘔",
  // 900
  u8"𝘕", u8"𝘖", u8"𝘗", u8"𝘘", u8"𝘙", u8"𝘚", u8"𝘛", u8"𝘜", u8"𝘝", u8"𝘞",
  u8"𝘟", u8"𝘠", u8"𝘡", u8"𝘼", u8"𝘽", u8"𝘾", u8"𝘿", u8"𝙀", u8"𝙁", u8"𝙂",
  u8"𝙃", u8"𝙄", u8"𝙅", u8"𝙆", u8"𝙇", u8"𝙈", u8"𝙉", u8"𝙊", u8"𝙋", u8"𝙌",
  u8"𝙍", u8"𝙎", u8"𝙏", u8"𝙐", u8"𝙑", u8"𝙒", u8"𝙓", u8"𝙔", u8"𝙕", u8"𝙰",
  u8"𝙱", u8"𝙲", u8"𝙳", u8"𝙴", u8"𝙵", u8"𝙶", u8"𝙷", u8"𝙸", u8"𝙹", u8"𝙺",
  u8"𝙻", u8"𝙼", u8"𝙽", u8"𝙾", u8"𝙿", u8"𝚀", u8"𝚁", u8"𝚂", u8"𝚃", u8"𝚄",
  u8"𝚅", u8"𝚆", u8"𝚇", u8"𝚈", u8"𝚉", u8"𝚨", u8"𝚩", u8"𝚪", u8"𝚫", u8"𝚬",
  u8"𝚭", u8"𝚮", u8"𝚯", u8"𝚰", u8"𝚱", u8"𝚲", u8"𝚳", u8"𝚴", u8"𝚵", u8"𝚶",
  u8"𝚷", u8"𝚸", u8"𝚺", u8"𝚻", u8"𝚼", u8"𝚽", u8"𝚾", u8"𝚿", u8"𝛀", u8"𝛢",
  u8"𝛣", u8"𝛤", u8"𝛥", u8"𝛦", u8"𝛧", u8"𝛨", u8"𝛩", u8"𝛪", u8"𝛫", u8"𝛬",
  // 1000
  u8"𝛭", u8"𝛮", u8"𝛯", u8"𝛰", u8"𝛱", u8"𝛲", u8"𝛴", u8"𝛵", u8"𝛶", u8"𝛷",
  u8"𝛸", u8"𝛹", u8"𝛺", u8"𝜜", u8"𝜝", u8"𝜞", u8"𝜟", u8"𝜠", u8"𝜡", u8"𝜢",
  u8"𝜣", u8"𝜤", u8"𝜥", u8"𝜦", u8"𝜧", u8"𝜨", u8"𝜩", u8"𝜪", u8"𝜫", u8"𝜬",
  u8"𝜮", u8"𝜯", u8"𝜰", u8"𝜱", u8"𝜲", u8"𝜳", u8"𝜴", u8"𝝖", u8"𝝗", u8"𝝘",
  u8"𝝙", u8"𝝚", u8"𝝛", u8"𝝜", u8"𝝝", u8"𝝞", u8"𝝟", u8"𝝠", u8"𝝡", u8"𝝢",
  u8"𝝣", u8"𝝤", u8"𝝥", u8"𝝦", u8"𝝨", u8"𝝩", u8"𝝪", u8"𝝫", u8"𝝬", u8"𝝭",
  u8"𝝮", u8"𝞐", u8"𝞑", u8"𝞒", u8"𝞓", u8"𝞔", u8"𝞕", u8"𝞖", u8"𝞗", u8"𝞘",
  u8"𝞙", u8"𝞚", u8"𝞛", u8"𝞜", u8"𝞝", u8"𝞞", u8"𝞟", u8"𝞠", u8"𝞢", u8"𝞣",
  u8"𝞤", u8"𝞥", u8"𝞦", u8"𝞧", u8"𝞨", u8"𝟊", u8"ℒ", u8"ℰ", u8"Ⅎ", u8"ℾ",
  u8"ℿ", u8"ⅅ", u8"Ꙁ", u8"Ꙃ", u8"Ꙅ", u8"Ꙇ", u8"Ꙉ", u8"Ꙋ", u8"Ꙍ", u8"Ꙏ",
  // 1100
  u8"Ꙑ", u8"Ꙓ", u8"Ꙕ", u8"Ꙗ", u8"Ꙙ", u8"Ꙛ", u8"Ꙝ", u8"Ꙟ", u8"Ꙡ", u8"Ꙣ",
  u8"Ꙥ", u8"Ꙧ", u8"Ꙩ", u8"Ꙫ", u8"Ꙭ", u8"Ꚁ", u8"Ꚃ", u8"Ꚅ", u8"Ꚇ", u8"Ꚉ",
  u8"Ꚋ", u8"Ꚍ", u8"Ꚏ", u8"Ꚑ", u8"Ꚓ", u8"Ꚕ", u8"Ꚗ", u8"Ꜣ", u8"Ꜥ", u8"Ꜧ",
  u8"Ꜩ", u8"Ꜫ", u8"Ꜭ", u8"Ꜯ", u8"Ꜳ", u8"Ꜵ", u8"Ꜷ", u8"Ꜹ", u8"Ꜻ", u8"Ꜽ",
  u8"Ꜿ", u8"Ꝁ", u8"Ꝃ", u8"Ꝅ", u8"Ꝇ", u8"Ꝉ", u8"Ꝋ", u8"Ꝍ", u8"Ꝏ", u8"Ꝑ",
  u8"Ꝓ", u8"Ꝕ", u8"Ꝗ", u8"Ꝙ", u8"Ꝛ", u8"Ꝝ", u8"Ꝟ", u8"Ꝡ", u8"Ꝣ", u8"Ꝥ",
  u8"Ꝧ", u8"Ꝩ", u8"Ꝫ", u8"Ꝭ", u8"Ꝯ", u8"Ꝺ", u8"Ꝼ", u8"Ᵹ", u8"Ꝿ", u8"Ꞁ",
  u8"Ꞃ", u8"Ꞅ", u8"Ꞇ", u8"Ꞌ", u8"Ɥ", u8"Ꞑ", u8"Ꞡ", u8"Ꞣ", u8"Ꞥ", u8"Ꞧ",
  u8"Ꞩ", u8"𐐀", u8"𐐁", u8"𐐂", u8"𐐃", u8"𐐄", u8"𐐅", u8"𐐆", u8"𐐇", u8"𐐈",
  u8"𐐉", u8"𐐊", u8"𐐋", u8"𐐌", u8"𐐍", u8"𐐎", u8"𐐏", u8"𐐐", u8"𐐑", u8"𐐒",
  // 1200
  u8"𐐓", u8"𐐔", u8"𐐕", u8"𐐖", u8"𐐗", u8"𐐘", u8"𐐙", u8"𐐚", u8"𐐛", u8"𐐜",
  u8"𐐝", u8"𐐞", u8"𐐟", u8"𐐠", u8"𐐡", u8"𐐢", u8"𐐣", u8"𐐤", u8"𐐥", u8"𐐦",
  u8"𐐧", u8"🄐", u8"🄑", u8"🄒", u8"🄓", u8"🄔", u8"🄕", u8"🄖", u8"🄗", u8"🄘",
  u8"🄙", u8"🄚", u8"🄛", u8"🄜", u8"🄝", u8"🄞", u8"🄟", u8"🄠", u8"🄡", u8"🄢",
  u8"🄣", u8"🄤", u8"🄥", u8"🄦", u8"🄧", u8"🄨", u8"🄩", u8"ᴬ", u8"ᴮ", u8"ᴰ",
  u8"ᴱ", u8"ᴳ", u8"ᴴ", u8"ᴶ", u8"ᴷ", u8"ᴸ", u8"ᴹ", u8"ᴼ", u8"ᴾ", u8"ᴿ",
  u8"ᵀ", u8"ᵁ", u8"ᵂ", u8"Ḁ", u8"Ḃ", u8"Ḅ", u8"Ḇ", u8"Ḉ", u8"Ḋ", u8"Ḍ",
  u8"Ḏ", u8"Ḑ", u8"Ḓ", u8"Ḕ", u8"Ḗ", u8"Ḙ", u8"Ḛ", u8"Ḝ", u8"Ḟ", u8"Ḡ",
  u8"Ḣ", u8"Ḥ", u8"Ḧ", u8"Ḩ", u8"Ḫ", u8"Ḭ", u8"Ḯ", u8"Ḱ", u8"Ḳ", u8"Ḵ",
  u8"Ḷ", u8"Ḹ", u8"Ḻ", u8"Ḽ", u8"Ḿ", u8"Ṁ", u8"Ṃ", u8"Ṅ", u8"Ṇ", u8"Ṉ",
  // 1300
  u8"Ṋ", u8"Ṍ", u8"Ṏ", u8"Ṑ", u8"Ṓ", u8"Ṕ", u8"Ṗ", u8"Ṙ", u8"Ṛ", u8"Ṝ",
  u8"Ṟ", u8"Ṡ", u8"Ṣ", u8"Ṥ", u8"Ṧ", u8"Ṩ", u8"Ṫ", u8"Ṭ", u8"Ṯ", u8"Ṱ",
  u8"Ṳ", u8"Ṵ", u8"Ṷ", u8"Ṹ", u8"Ṻ", u8"Ṽ", u8"Ṿ", u8"Ẁ", u8"Ẃ", u8"Ẅ",
  u8"Ẇ", u8"Ẉ", u8"Ẋ", u8"Ẍ", u8"Ẏ", u8"Ẑ", u8"Ẓ", u8"Ẕ", u8"ẞ", u8"Ạ",
  u8"Ả", u8"Ấ", u8"Ầ", u8"Ẩ", u8"Ẫ", u8"Ậ", u8"Ắ", u8"Ằ", u8"Ẳ", u8"Ẵ",
  u8"Ặ", u8"Ẹ", u8"Ẻ", u8"Ẽ", u8"Ế", u8"Ề", u8"Ể", u8"Ễ", u8"Ệ", u8"Ỉ",
  u8"Ị", u8"Ọ", u8"Ỏ", u8"Ố", u8"Ồ", u8"Ổ", u8"Ỗ", u8"Ộ", u8"Ớ", u8"Ờ",
  u8"Ở", u8"Ỡ", u8"Ợ", u8"Ụ", u8"Ủ", u8"Ứ", u8"Ừ", u8"Ử", u8"Ữ", u8"Ự",
  u8"Ỳ", u8"Ỵ", u8"Ỷ", u8"Ỹ", u8"Ỻ", u8"Ỽ", u8"Ỿ", u8"Ἀ", u8"Ἁ", u8"Ἂ",
  u8"Ἃ", u8"Ἄ", u8"Ἅ", u8"Ἆ", u8"Ἇ", u8"Ἐ", u8"Ἑ", u8"Ἒ", u8"Ἓ", u8"Ἔ",
  // 1400
  u8"Ἕ", u8"Ἠ", u8"Ἡ", u8"Ἢ", u8"Ἣ", u8"Ἤ", u8"Ἥ", u8"Ἦ", u8"Ἧ", u8"Ἰ",
  u8"Ἱ", u8"Ἲ", u8"Ἳ", u8"Ἴ", u8"Ἵ", u8"Ἶ", u8"Ἷ", u8"Ὀ", u8"Ὁ", u8"Ὂ",
  u8"Ὃ", u8"Ὄ", u8"Ὅ", u8"Ὑ", u8"Ὓ", u8"Ὕ", u8"Ὗ", u8"Ὠ", u8"Ὡ", u8"Ὢ",
  u8"Ὣ", u8"Ὤ", u8"Ὥ", u8"Ὦ", u8"Ὧ", u8"Ᾰ", u8"Ᾱ", u8"Ὰ", u8"Ά", u8"Ὲ",
  u8"Έ", u8"Ὴ", u8"Ή", u8"Ῐ", u8"Ῑ", u8"Ὶ", u8"Ί", u8"Ῠ", u8"Ῡ", u8"Ὺ",
  u8"Ύ", u8"Ῥ", u8"Ὸ", u8"Ό", u8"Ὼ", u8"Ώ"
};

const NAUTF8Char* na_u8Lowers[NA_U8_UPPER_LOWER_COUNT] = {
  // 0
  u8"ѐ", u8"ё", u8"ђ", u8"ѓ", u8"є", u8"ѕ", u8"і", u8"ї", u8"ј", u8"љ",
  u8"њ", u8"ћ", u8"ќ", u8"ѝ", u8"ў", u8"џ", u8"а", u8"б", u8"в", u8"г",
  u8"д", u8"е", u8"ж", u8"з", u8"и", u8"й", u8"к", u8"л", u8"м", u8"н",
  u8"о", u8"п", u8"р", u8"с", u8"т", u8"у", u8"ф", u8"х", u8"ц", u8"ч",
  u8"ш", u8"щ", u8"ъ", u8"ы", u8"ь", u8"э", u8"ю", u8"я", u8"ѡ", u8"ѣ",
  u8"ѥ", u8"ѧ", u8"ѩ", u8"ѫ", u8"ѭ", u8"ѯ", u8"ѱ", u8"ѳ", u8"ѵ", u8"ѷ",
  u8"ѹ", u8"ѻ", u8"ѽ", u8"ѿ", u8"ҁ", u8"ҋ", u8"ҍ", u8"ҏ", u8"ґ", u8"ғ",
  u8"ҕ", u8"җ", u8"ҙ", u8"қ", u8"ҝ", u8"ҟ", u8"ҡ", u8"ң", u8"ҥ", u8"ҧ",
  u8"ҩ", u8"ҫ", u8"ҭ", u8"ү", u8"ұ", u8"ҳ", u8"ҵ", u8"ҷ", u8"ҹ", u8"һ",
  u8"ҽ", u8"ҿ", u8"ӂ", u8"ӄ", u8"ӆ", u8"ӈ", u8"ӊ", u8"ӌ", u8"ӎ", u8"ӑ",
  // 100
  u8"ӓ", u8"ӕ", u8"ӗ", u8"ә", u8"ӛ", u8"ӝ", u8"ӟ", u8"ӡ", u8"ӣ", u8"ӥ",
  u8"ӧ", u8"ө", u8"ӫ", u8"ӭ", u8"ӯ", u8"ӱ", u8"ӳ", u8"ӵ", u8"ӷ", u8"ӹ",
  u8"ӻ", u8"ӽ", u8"ӿ", u8"ԁ", u8"ԃ", u8"ԅ", u8"ԇ", u8"ԉ", u8"ԋ", u8"ԍ",
  u8"ԏ", u8"ԑ", u8"ԓ", u8"ԕ", u8"ԗ", u8"ԙ", u8"ԛ", u8"ԝ", u8"ԟ", u8"ԡ",
  u8"ԣ", u8"ԥ", u8"ԧ", u8"ա", u8"բ", u8"գ", u8"դ", u8"ե", u8"զ", u8"է",
  u8"ը", u8"թ", u8"ժ", u8"ի", u8"լ", u8"խ", u8"ծ", u8"կ", u8"հ", u8"ձ",
  u8"ղ", u8"ճ", u8"մ", u8"յ", u8"ն", u8"շ", u8"ո", u8"չ", u8"պ", u8"ջ",
  u8"ռ", u8"ս", u8"վ", u8"տ", u8"ր", u8"ց", u8"ւ", u8"փ", u8"ք", u8"օ",
  u8"ֆ", u8"a", u8"b", u8"c", u8"d", u8"e", u8"f", u8"g", u8"h", u8"i",
  u8"j", u8"k", u8"l", u8"m", u8"n", u8"o", u8"p", u8"q", u8"r", u8"s",
  // 200
  u8"t", u8"u", u8"v", u8"w", u8"x", u8"y", u8"z", u8"à", u8"á", u8"â",
  u8"ã", u8"ä", u8"å", u8"æ", u8"ç", u8"è", u8"é", u8"ê", u8"ë", u8"ì",
  u8"í", u8"î", u8"ï", u8"ð", u8"ñ", u8"ò", u8"ó", u8"ô", u8"õ", u8"ö",
  u8"ø", u8"ù", u8"ú", u8"û", u8"ü", u8"ý", u8"þ", u8"ā", u8"ă", u8"ą",
  u8"ć", u8"ĉ", u8"ċ", u8"č", u8"ď", u8"đ", u8"ē", u8"ĕ", u8"ė", u8"ę",
  u8"ě", u8"ĝ", u8"ğ", u8"ġ", u8"ģ", u8"ĥ", u8"ħ", u8"ĩ", u8"ī", u8"ĭ",
  u8"į", u8"ĳ", u8"ĵ", u8"ķ", u8"ĺ", u8"ļ", u8"ľ", u8"ŀ", u8"ł", u8"ń",
  u8"ņ", u8"ň", u8"ŋ", u8"ō", u8"ŏ", u8"ő", u8"œ", u8"ŕ", u8"ŗ", u8"ř",
  u8"ś", u8"ŝ", u8"ş", u8"š", u8"ţ", u8"ť", u8"ŧ", u8"ũ", u8"ū", u8"ŭ",
  u8"ů", u8"ű", u8"ų", u8"ŵ", u8"ŷ", u8"ÿ", u8"ź", u8"ż", u8"ž", u8"ɓ",
  // 300
  u8"ƃ", u8"ƅ", u8"ɔ", u8"ƈ", u8"ɗ", u8"ƌ", u8"ɘ", u8"ə", u8"ɛ", u8"ƒ",
  u8"ɠ", u8"ɣ", u8"ɩ", u8"ɨ", u8"ƙ", u8"ɯ", u8"ɲ", u8"ơ", u8"ƣ", u8"ƥ",
  u8"ƨ", u8"ʃ", u8"ƭ", u8"ʈ", u8"ư", u8"ʊ", u8"ʋ", u8"ƴ", u8"ƶ", u8"ʒ",
  u8"ƹ", u8"ƽ", u8"ǆ", u8"ǉ", u8"ǌ", u8"ǎ", u8"ǐ", u8"ǒ", u8"ǔ", u8"ǖ",
  u8"ǘ", u8"ǚ", u8"ǜ", u8"ǟ", u8"ǡ", u8"ǣ", u8"ǥ", u8"ǧ", u8"ǩ", u8"ǫ",
  u8"ǭ", u8"ǯ", u8"ǳ", u8"ǵ", u8"ǹ", u8"ǻ", u8"ǽ", u8"ǿ", u8"ȁ", u8"ȃ",
  u8"ȅ", u8"ȇ", u8"ȉ", u8"ȋ", u8"ȍ", u8"ȏ", u8"ȑ", u8"ȓ", u8"ȕ", u8"ȗ",
  u8"ș", u8"ț", u8"ȝ", u8"ȟ", u8"ƞ", u8"ȣ", u8"ȥ", u8"ȧ", u8"ȩ", u8"ȫ",
  u8"ȭ", u8"ȯ", u8"ȱ", u8"ȳ", u8"ⱥ", u8"ȼ", u8"ƚ", u8"ⱦ", u8"ɂ", u8"ƀ",
  u8"ʉ", u8"ʌ", u8"ɇ", u8"ɉ", u8"ɍ", u8"ɏ", u8"ͱ", u8"ͳ", u8"ͷ", u8"ά",
  // 400
  u8"έ", u8"ή", u8"ί", u8"ό", u8"ύ", u8"ώ", u8"α", u8"β", u8"γ", u8"δ",
  u8"ε", u8"ζ", u8"η", u8"θ", u8"ι", u8"κ", u8"λ", u8"μ", u8"ν", u8"ξ",
  u8"ο", u8"π", u8"ρ", u8"σ", u8"τ", u8"υ", u8"φ", u8"χ", u8"ψ", u8"ω",
  u8"ϊ", u8"ϋ", u8"ϣ", u8"ϥ", u8"ϧ", u8"ϩ", u8"ϫ", u8"ϭ", u8"ϯ", u8"ϸ",
  u8"ϻ", u8"ͻ", u8"ͼ", u8"ͽ", u8"ⴀ", u8"ⴁ", u8"ⴂ", u8"ⴃ", u8"ⴄ", u8"ⴅ",
  u8"ⴆ", u8"ⴇ", u8"ⴈ", u8"ⴉ", u8"ⴊ", u8"ⴋ", u8"ⴌ", u8"ⴍ", u8"ⴎ", u8"ⴏ",
  u8"ⴐ", u8"ⴑ", u8"ⴒ", u8"ⴓ", u8"ⴔ", u8"ⴕ", u8"ⴖ", u8"ⴗ", u8"ⴘ", u8"ⴙ",
  u8"ⴚ", u8"ⴛ", u8"ⴜ", u8"ⴝ", u8"ⴞ", u8"ⴟ", u8"ⴠ", u8"ⴡ", u8"ⴢ", u8"ⴣ",
  u8"ⴤ", u8"ⴥ", u8"🔡", u8"ａ", u8"ｂ", u8"ｃ", u8"ｄ", u8"ｅ", u8"ｆ", u8"ｇ",
  u8"ｈ", u8"ｉ", u8"ｊ", u8"ｋ", u8"ｌ", u8"ｍ", u8"ｎ", u8"ｏ", u8"ｐ", u8"ｑ",
  // 500
  u8"ｒ", u8"ｓ", u8"ｔ", u8"ｕ", u8"ｖ", u8"ｗ", u8"ｘ", u8"ｙ", u8"ｚ", u8"ⓐ",
  u8"ⓑ", u8"ⓒ", u8"ⓓ", u8"ⓔ", u8"ⓕ", u8"ⓖ", u8"ⓗ", u8"ⓘ", u8"ⓙ", u8"ⓚ",
  u8"ⓛ", u8"ⓜ", u8"ⓝ", u8"ⓞ", u8"ⓟ", u8"ⓠ", u8"ⓡ", u8"ⓢ", u8"ⓣ", u8"ⓤ",
  u8"ⓥ", u8"ⓦ", u8"ⓧ", u8"ⓨ", u8"ⓩ", u8"ⰰ", u8"ⰱ", u8"ⰲ", u8"ⰳ", u8"ⰴ",
  u8"ⰵ", u8"ⰶ", u8"ⰷ", u8"ⰸ", u8"ⰹ", u8"ⰺ", u8"ⰻ", u8"ⰼ", u8"ⰽ", u8"ⰾ",
  u8"ⰿ", u8"ⱀ", u8"ⱁ", u8"ⱂ", u8"ⱃ", u8"ⱄ", u8"ⱅ", u8"ⱆ", u8"ⱇ", u8"ⱈ",
  u8"ⱉ", u8"ⱊ", u8"ⱋ", u8"ⱌ", u8"ⱍ", u8"ⱎ", u8"ⱏ", u8"ⱐ", u8"ⱑ", u8"ⱒ",
  u8"ⱓ", u8"ⱔ", u8"ⱕ", u8"ⱖ", u8"ⱗ", u8"ⱘ", u8"ⱙ", u8"ⱚ", u8"ⱛ", u8"ⱜ",
  u8"ⱝ", u8"ⱞ", u8"ⱡ", u8"ɫ", u8"ᵽ", u8"ɽ", u8"ⱨ", u8"ⱪ", u8"ⱬ", u8"ɑ",
  u8"ɱ", u8"ɐ", u8"ɒ", u8"ⱳ", u8"ⱶ", u8"ᵛ", u8"ȿ", u8"ɀ", u8"ⲁ", u8"ⲃ",
  // 600
  u8"ⲅ", u8"ⲇ", u8"ⲉ", u8"ⲋ", u8"ⲍ", u8"ⲏ", u8"ⲑ", u8"ⲓ", u8"ⲕ", u8"ⲗ",
  u8"ⲙ", u8"ⲛ", u8"ⲝ", u8"ⲟ", u8"ⲡ", u8"ⲣ", u8"ⲥ", u8"ⲧ", u8"ⲩ", u8"ⲫ",
  u8"ⲭ", u8"ⲯ", u8"ⲱ", u8"ⲳ", u8"ⲵ", u8"ⲷ", u8"ⲹ", u8"ⲻ", u8"ⲽ", u8"ⲿ",
  u8"ⳁ", u8"ⳃ", u8"ⳅ", u8"ⳇ", u8"ⳉ", u8"ⳋ", u8"ⳍ", u8"ⳏ", u8"ⳑ", u8"ⳓ",
  u8"ⳕ", u8"ⳗ", u8"ⳙ", u8"ⳛ", u8"ⳝ", u8"ⳟ", u8"ⳡ", u8"ⳣ", u8"ⳬ", u8"ⳮ",
  u8"𝐚", u8"𝐛", u8"𝐜", u8"𝐝", u8"𝐞", u8"𝐟", u8"𝐠", u8"𝐡", u8"𝐢", u8"𝐣",
  u8"𝐤", u8"𝐥", u8"𝐦", u8"𝐧", u8"𝐨", u8"𝐩", u8"𝐪", u8"𝐫", u8"𝐬", u8"𝐭",
  u8"𝐮", u8"𝐯", u8"𝐰", u8"𝐱", u8"𝐲", u8"𝐳", u8"𝑎", u8"𝑏", u8"𝑐", u8"𝑑",
  u8"𝑒", u8"𝑓", u8"𝑔", u8"𝑖", u8"𝑗", u8"𝑘", u8"𝑙", u8"𝑚", u8"𝑛", u8"𝑜",
  u8"𝑝", u8"𝑞", u8"𝑟", u8"𝑠", u8"𝑡", u8"𝑢", u8"𝑣", u8"𝑤", u8"𝑥", u8"𝑦",
  // 700
  u8"𝑧", u8"𝒂", u8"𝒃", u8"𝒄", u8"𝒅", u8"𝒆", u8"𝒇", u8"𝒈", u8"𝒉", u8"𝒊",
  u8"𝒋", u8"𝒌", u8"𝒍", u8"𝒎", u8"𝒏", u8"𝒐", u8"𝒑", u8"𝒒", u8"𝒓", u8"𝒔",
  u8"𝒕", u8"𝒖", u8"𝒗", u8"𝒘", u8"𝒙", u8"𝒚", u8"𝒛", u8"𝒶", u8"𝒸", u8"𝒹",
  u8"𝒿", u8"𝓀", u8"𝓃", u8"𝓅", u8"𝓆", u8"𝓈", u8"𝓉", u8"𝓊", u8"𝓋", u8"𝓌",
  u8"𝓍", u8"𝓎", u8"𝓏", u8"𝓪", u8"𝓫", u8"𝓬", u8"𝓭", u8"𝓮", u8"𝓯", u8"𝓰",
  u8"𝓱", u8"𝓲", u8"𝓳", u8"𝓴", u8"𝓵", u8"𝓶", u8"𝓷", u8"𝓸", u8"𝓹", u8"𝓺",
  u8"𝓻", u8"𝓼", u8"𝓽", u8"𝓾", u8"𝓿", u8"𝔀", u8"𝔁", u8"𝔂", u8"𝔃", u8"𝔞",
  u8"𝔟", u8"𝔡", u8"𝔢", u8"𝔣", u8"𝔤", u8"𝔧", u8"𝔨", u8"𝔩", u8"𝔪", u8"𝔫",
  u8"𝔬", u8"𝔭", u8"𝔮", u8"𝔰", u8"𝔱", u8"𝔲", u8"𝔳", u8"𝔴", u8"𝔵", u8"𝔶",
  u8"𝕒", u8"𝕓", u8"𝕕", u8"𝕖", u8"𝕗", u8"𝕘", u8"𝕚", u8"𝕛", u8"𝕜", u8"𝕝",
  // 800
  u8"𝕞", u8"𝕠", u8"𝕤", u8"𝕥", u8"𝕦", u8"𝕧", u8"𝕨", u8"𝕩", u8"𝕪", u8"𝖆",
  u8"𝖇", u8"𝖈", u8"𝖉", u8"𝖊", u8"𝖋", u8"𝖌", u8"𝖍", u8"𝖎", u8"𝖏", u8"𝖐",
  u8"𝖑", u8"𝖒", u8"𝖓", u8"𝖔", u8"𝖕", u8"𝖖", u8"𝖗", u8"𝖘", u8"𝖙", u8"𝖚",
  u8"𝖛", u8"𝖜", u8"𝖝", u8"𝖞", u8"𝖟", u8"𝖺", u8"𝖻", u8"𝖼", u8"𝖽", u8"𝖾",
  u8"𝖿", u8"𝗀", u8"𝗁", u8"𝗂", u8"𝗃", u8"𝗄", u8"𝗅", u8"𝗆", u8"𝗇", u8"𝗈",
  u8"𝗉", u8"𝗊", u8"𝗋", u8"𝗌", u8"𝗍", u8"𝗎", u8"𝗏", u8"𝗐", u8"𝗑", u8"𝗒",
  u8"𝗓", u8"𝗮", u8"𝗯", u8"𝗰", u8"𝗱", u8"𝗲", u8"𝗳", u8"𝗴", u8"𝗵", u8"𝗶",
  u8"𝗷", u8"𝗸", u8"𝗹", u8"𝗺", u8"𝗻", u8"𝗼", u8"𝗽", u8"𝗾", u8"𝗿", u8"𝘀",
  u8"𝘁", u8"𝘂", u8"𝘃", u8"𝘄", u8"𝘅", u8"𝘆", u8"𝘇", u8"𝘢", u8"𝘣", u8"𝘤",
  u8"𝘥", u8"𝘦", u8"𝘧", u8"𝘨", u8"𝘩", u8"𝘪", u8"𝘫", u8"𝘬", u8"𝘭", u8"𝘮",
  // 900
  u8"𝘯", u8"𝘰", u8"𝘱", u8"𝘲", u8"𝘳", u8"𝘴", u8"𝘵", u8"𝘶", u8"𝘷", u8"𝘸",
  u8"𝘹", u8"𝘺", u8"𝘻", u8"𝙖", u8"𝙗", u8"𝙘", u8"𝙙", u8"𝙚", u8"𝙛", u8"𝙜",
  u8"𝙝", u8"𝙞", u8"𝙟", u8"𝙠", u8"𝙡", u8"𝙢", u8"𝙣", u8"𝙤", u8"𝙥", u8"𝙦",
  u8"𝙧", u8"𝙨", u8"𝙩", u8"𝙪", u8"𝙫", u8"𝙬", u8"𝙭", u8"𝙮", u8"𝙯", u8"𝚊",
  u8"𝚋", u8"𝚌", u8"𝚍", u8"𝚎", u8"𝚏", u8"𝚐", u8"𝚑", u8"𝚒", u8"𝚓", u8"𝚔",
  u8"𝚕", u8"𝚖", u8"𝚗", u8"𝚘", u8"𝚙", u8"𝚚", u8"𝚛", u8"𝚜", u8"𝚝", u8"𝚞",
  u8"𝚟", u8"𝚠", u8"𝚡", u8"𝚢", u8"𝚣", u8"𝛂", u8"𝛃", u8"𝛄", u8"𝛅", u8"𝛆",
  u8"𝛇", u8"𝛈", u8"𝛉", u8"𝛊", u8"𝛋", u8"𝛌", u8"𝛍", u8"𝛎", u8"𝛏", u8"𝛐",
  u8"𝛑", u8"𝛒", u8"𝛔", u8"𝛕", u8"𝛖", u8"𝛗", u8"𝛘", u8"𝛙", u8"𝛚", u8"𝛼",
  u8"𝛽", u8"𝛾", u8"𝛿", u8"𝜀", u8"𝜁", u8"𝜂", u8"𝜃", u8"𝜄", u8"𝜅", u8"𝜆",
  // 1000
  u8"𝜇", u8"𝜈", u8"𝜉", u8"𝜊", u8"𝜋", u8"𝜌", u8"𝜎", u8"𝜏", u8"𝜐", u8"𝜑",
  u8"𝜒", u8"𝜓", u8"𝜔", u8"𝜶", u8"𝜷", u8"𝜸", u8"𝜹", u8"𝜺", u8"𝜻", u8"𝜼",
  u8"𝜽", u8"𝜾", u8"𝜿", u8"𝝀", u8"𝝁", u8"𝝂", u8"𝝃", u8"𝝄", u8"𝝅", u8"𝝆",
  u8"𝝈", u8"𝝉", u8"𝝊", u8"𝝋", u8"𝝌", u8"𝝍", u8"𝝎", u8"𝝰", u8"𝝱", u8"𝝲",
  u8"𝝳", u8"𝝴", u8"𝝵", u8"𝝶", u8"𝝷", u8"𝝸", u8"𝝹", u8"𝝺", u8"𝝻", u8"𝝼",
  u8"𝝽", u8"𝝾", u8"𝝿", u8"𝞀", u8"𝞂", u8"𝞃", u8"𝞄", u8"𝞅", u8"𝞆", u8"𝞇",
  u8"𝞈", u8"𝞪", u8"𝞫", u8"𝞬", u8"𝞭", u8"𝞮", u8"𝞯", u8"𝞰", u8"𝞱", u8"𝞲",
  u8"𝞳", u8"𝞴", u8"𝞵", u8"𝞶", u8"𝞷", u8"𝞸", u8"𝞹", u8"𝞺", u8"𝞼", u8"𝞽",
  u8"𝞾", u8"𝞿", u8"𝟀", u8"𝟁", u8"𝟂", u8"𝟋", u8"ℓ", u8"ℯ", u8"ⅎ", u8"ℽ",
  u8"ℼ", u8"ⅆ", u8"ꙁ", u8"ꙃ", u8"ꙅ", u8"ꙇ", u8"ꙉ", u8"ꙋ", u8"ꙍ", u8"ꙏ",
  // 1100
  u8"ꙑ", u8"ꙓ", u8"ꙕ", u8"ꙗ", u8"ꙙ", u8"ꙛ", u8"ꙝ", u8"ꙟ", u8"ꙡ", u8"ꙣ",
  u8"ꙥ", u8"ꙧ", u8"ꙩ", u8"ꙫ", u8"ꙭ", u8"ꚁ", u8"ꚃ", u8"ꚅ", u8"ꚇ", u8"ꚉ",
  u8"ꚋ", u8"ꚍ", u8"ꚏ", u8"ꚑ", u8"ꚓ", u8"ꚕ", u8"ꚗ", u8"ꜣ", u8"ꜥ", u8"ꜧ",
  u8"ꜩ", u8"ꜫ", u8"ꜭ", u8"ꜯ", u8"ꜳ", u8"ꜵ", u8"ꜷ", u8"ꜹ", u8"ꜻ", u8"ꜽ",
  u8"ꜿ", u8"ꝁ", u8"ꝃ", u8"ꝅ", u8"ꝇ", u8"ꝉ", u8"ꝋ", u8"ꝍ", u8"ꝏ", u8"ꝑ",
  u8"ꝓ", u8"ꝕ", u8"ꝗ", u8"ꝙ", u8"ꝛ", u8"ꝝ", u8"ꝟ", u8"ꝡ", u8"ꝣ", u8"ꝥ",
  u8"ꝧ", u8"ꝩ", u8"ꝫ", u8"ꝭ", u8"ꝯ", u8"ꝺ", u8"ꝼ", u8"ᵹ", u8"ꝿ", u8"ꞁ",
  u8"ꞃ", u8"ꞅ", u8"ꞇ", u8"ꞌ", u8"ɥ", u8"ꞑ", u8"ꞡ", u8"ꞣ", u8"ꞥ", u8"ꞧ",
  u8"ꞩ", u8"𐐨", u8"𐐩", u8"𐐪", u8"𐐫", u8"𐐬", u8"𐐭", u8"𐐮", u8"𐐯", u8"𐐰",
  u8"𐐱", u8"𐐲", u8"𐐳", u8"𐐴", u8"𐐵", u8"𐐶", u8"𐐷", u8"𐐸", u8"𐐹", u8"𐐺",
  // 1200
  u8"𐐻", u8"𐐼", u8"𐐽", u8"𐐾", u8"𐐿", u8"𐑀", u8"𐑁", u8"𐑂", u8"𐑃", u8"𐑄",
  u8"𐑅", u8"𐑆", u8"𐑇", u8"𐑈", u8"𐑉", u8"𐑊", u8"𐑋", u8"𐑌", u8"𐑍", u8"𐑎",
  u8"𐑏", u8"⒜", u8"⒝", u8"⒞", u8"⒟", u8"⒠", u8"⒡", u8"⒢", u8"⒣", u8"⒤",
  u8"⒥", u8"⒦", u8"⒧", u8"⒨", u8"⒩", u8"⒪", u8"⒫", u8"⒬", u8"⒭", u8"⒮",
  u8"⒯", u8"⒰", u8"⒱", u8"⒲", u8"⒳", u8"⒴", u8"⒵", u8"ᵃ", u8"ᵇ", u8"ᵈ",
  u8"ᵉ", u8"ᵍ", u8"ʰ", u8"ʲ", u8"ᵏ", u8"ˡ", u8"ᵐ", u8"ᵒ", u8"ᵖ", u8"ʳ",
  u8"ᵗ", u8"ᵘ", u8"ʷ", u8"ḁ", u8"ḃ", u8"ḅ", u8"ḇ", u8"ḉ", u8"ḋ", u8"ḍ",
  u8"ḏ", u8"ḑ", u8"ḓ", u8"ḕ", u8"ḗ", u8"ḙ", u8"ḛ", u8"ḝ", u8"ḟ", u8"ḡ",
  u8"ḣ", u8"ḥ", u8"ḧ", u8"ḩ", u8"ḫ", u8"ḭ", u8"ḯ", u8"ḱ", u8"ḳ", u8"ḵ",
  u8"ḷ", u8"ḹ", u8"ḻ", u8"ḽ", u8"ḿ", u8"ṁ", u8"ṃ", u8"ṅ", u8"ṇ", u8"ṉ",
  // 1300
  u8"ṋ", u8"ṍ", u8"ṏ", u8"ṑ", u8"ṓ", u8"ṕ", u8"ṗ", u8"ṙ", u8"ṛ", u8"ṝ",
  u8"ṟ", u8"ṡ", u8"ṣ", u8"ṥ", u8"ṧ", u8"ṩ", u8"ṫ", u8"ṭ", u8"ṯ", u8"ṱ",
  u8"ṳ", u8"ṵ", u8"ṷ", u8"ṹ", u8"ṻ", u8"ṽ", u8"ṿ", u8"ẁ", u8"ẃ", u8"ẅ",
  u8"ẇ", u8"ẉ", u8"ẋ", u8"ẍ", u8"ẏ", u8"ẑ", u8"ẓ", u8"ẕ", u8"ß", u8"ạ",
  u8"ả", u8"ấ", u8"ầ", u8"ẩ", u8"ẫ", u8"ậ", u8"ắ", u8"ằ", u8"ẳ", u8"ẵ",
  u8"ặ", u8"ẹ", u8"ẻ", u8"ẽ", u8"ế", u8"ề", u8"ể", u8"ễ", u8"ệ", u8"ỉ",
  u8"ị", u8"ọ", u8"ỏ", u8"ố", u8"ồ", u8"ổ", u8"ỗ", u8"ộ", u8"ớ", u8"ờ",
  u8"ở", u8"ỡ", u8"ợ", u8"ụ", u8"ủ", u8"ứ", u8"ừ", u8"ử", u8"ữ", u8"ự",
  u8"ỳ", u8"ỵ", u8"ỷ", u8"ỹ", u8"ỻ", u8"ỽ", u8"ỿ", u8"ἀ", u8"ἁ", u8"ἂ",
  u8"ἃ", u8"ἄ", u8"ἅ", u8"ἆ", u8"ἇ", u8"ἐ", u8"ἑ", u8"ἒ", u8"ἓ", u8"ἔ",
  // 1400
  u8"ἕ", u8"ἠ", u8"ἡ", u8"ἢ", u8"ἣ", u8"ἤ", u8"ἥ", u8"ἦ", u8"ἧ", u8"ἰ",
  u8"ἱ", u8"ἲ", u8"ἳ", u8"ἴ", u8"ἵ", u8"ἶ", u8"ἷ", u8"ὀ", u8"ὁ", u8"ὂ",
  u8"ὃ", u8"ὄ", u8"ὅ", u8"ὑ", u8"ὓ", u8"ὕ", u8"ὗ", u8"ὠ", u8"ὡ", u8"ὢ",
  u8"ὣ", u8"ὤ", u8"ὥ", u8"ὦ", u8"ὧ", u8"ᾰ", u8"ᾱ", u8"ὰ", u8"ά", u8"ὲ",
  u8"έ", u8"ὴ", u8"ή", u8"ῐ", u8"ῑ", u8"ὶ", u8"ί", u8"ῠ", u8"ῡ", u8"ὺ",
  u8"ύ", u8"ῥ", u8"ὸ", u8"ό", u8"ὼ", u8"ώ"
};



// This is not a very fast method. It uses brute force over 1500 strings.
NA_DEF const NAUTF8Char* naConvertUTF8CharToUppercase(const NAUTF8Char* lower) {
  for(size_t i = 0; i < NA_U8_UPPER_LOWER_COUNT; ++i) {
    if(naEqualUTF8CStringLiterals(lower, na_u8Lowers[i], 0, NA_TRUE)) {
      return na_u8Uppers[i];
    }
  }
  return lower;
}



// This is not a very fast method. It uses brute force over 1500 strings.
NA_DEF const NAUTF8Char* naConvertUTF8CharToLowercase(const NAUTF8Char* upper) {
  for(size_t i = 0; i < NA_U8_UPPER_LOWER_COUNT; ++i) {
    if(naEqualUTF8CStringLiterals(upper, na_u8Uppers[i], 0, NA_TRUE)) {
      return na_u8Lowers[i];
    }
  }
  return upper;
}
