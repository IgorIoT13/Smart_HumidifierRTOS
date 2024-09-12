#ifndef INDEX_HTML_H
#define INDEX_HTML_H


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
<meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin: 0px auto;
            padding-top: 30px;
        }

        .button {
            padding: 10px 20px;
            margin-bottom: 50px;
            font-size: 24px;
            text-align: center;
            outline: none;
            color: #fff;
            background-color: #2f4468;
            border: none;
            border-radius: 5px;
            box-shadow: 0 6px #999;
            cursor: pointer;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        .button:hover {
            background-color: #1f2e45
        }

        .button:active {
            background-color: #1f2e45;
            box-shadow: 0 4px #666;
            transform: translateY(2px);
        }

        .input1 {
            margin-bottom: 30px;
            padding: 6px 12px;
            font-size: 16px;
            font-weight: 700;
            line-height: 1.5;
            color: #212529;
            background-color: #fff;
            background-clip: padding-box;
            border: 1px solid #ced4da;
            appearance: none;
            border-radius: 4px;
            transition: border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .label1 {
            font-size: 28px;
        }

        .custom_margin {
            padding-bottom: 30px;

        }

        @media screen and (max-width: 480px) {
            .button {
                padding: 15px 100px 15px 10px;
                font-size: 10px;
            }

            h1 {
                font-size: 24px;
                padding-top: 20px;
            }
        }
    </style>
</head>

<body>
    <h1>ESP освіжувач повітря</h1>
    <div>
        <label for="delay" class="label1">Температура - <label id="temperatureValue"></label></label>

    </div>
    <div class="custom_margin"></div>
    <div>
        <label for="delay" class="label1">Вологість - <label id="humidityValue"></label></label>

    </div>
    <div class="custom_margin"></div>
    <div>
        <button class="button" id="wifiButton" onclick="sendDataAccess();">
            Чи надсилати дані по WiFi - <span id="wifiStatus">так</span>
        </button>
    </div>
    <div>
        <button class="button" id="rgbButton" onclick="sendRgbStatus();">
            Чи ввімкнути підсвітку - <span id="rgbStatus">так</span>
        </button>
    </div>
    <div>
        <label for="delay" class="label1">Затримка між перевіркою температури (в секундах), поточне - <label
                id="tempCheck"></label>: </label>
        <input type="number" class="input1" id="temperature" oninput="validateIntegerInput(this)" min="1" max="120">
        <button class="button" onclick="sendTemperatureData()">Надіслати</button>
    </div>
    <div>
        <label for="delay" class="label1">Час роботи зволожувача повітря (в секундах), поточне - <label
                id="humidCheck"></label>: </label>
        <input type="number" class="input1" id="humidifierWorking" oninput="validateIntegerInput(this)" min="1"
            max="120">
        <button class="button" onclick="sendHumidifierWorkingTime()">Надіслати</button>
    </div>
    <div>
        <label for="delay" class="label1">Час затримки між зволоженнями (в секундах), поточне - <label
                id="delayHumid"></label>: </label>
        <input type="number" class="input1" id="humidifierSleeping" oninput="validateIntegerInput(this)" min="1"
            max="120">
        <button class="button" onclick="sendHumidifierSleepingTime()">Надіслати</button>
    </div>

    <div>
        <label for="levelHumidity" class="label1">Рівень вологості, нижче якого розпочнеться зволоження, поточне -
            <label id="humidLevel"></label>: </label>
        <input type="number" class="input1" id="humidity" oninput="validateIntegerInput(this)" min="1" max="100">
        <button class="button" onclick="sendHumidityData()">Надіслати</button>
    </div>

    <script>
        var temperature = document.getElementById('temperature');
        var humidity = document.getElementById('humidity');
        var humidifierWorkingTime = document.getElementById('humidifierWorking');
        var humidifierSleepingTime = document.getElementById('humidifierSleeping');


        function validateIntegerInput(input) {
            input.value = input.value.replace(/[^0-9]/g, '');
        }

        function sendDataAccess() {
            var wifiStatus = document.getElementById('wifiStatus');
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/", true);
            xhr.send();
            if (wifiStatus.textContent === 'так') {
                xhr.open("GET", "/wifi_false", true);
                xhr.send();
                wifiStatus.textContent = 'ні';
            } else {
                xhr.open("GET", "/wifi_true", true);
                xhr.send();
                wifiStatus.textContent = 'так';
            }
        }
        function sendRgbStatus() {
            var rgbStatus = document.getElementById('rgbStatus');
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/", true);
            xhr.send();
            if (rgbStatus.textContent === 'так') {
                xhr.open("GET", "/rgb_false", true);
                xhr.send();
                rgbStatus.textContent = 'ні';
            } else {
                xhr.open("GET", "/rgb_true", true);
                xhr.send();
                rgbStatus.textContent = 'так';
            }
        }

        function sendHumidifierWorkingTime() {
            var humWorking = humidifierWorkingTime.value;
            if (humWorking >= 1 && humWorking <= 120) {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/set_humidifier_working_time?value=" + humWorking, true);
                xhr.send();
            } else {
                alert("Вкажіть затримку між переваркою в межах від 1 до 120");
            }
        }

        function sendHumidifierSleepingTime() {
            var humSleeping = humidifierSleepingTime.value;
            if (humSleeping >= 1 && humSleeping <= 120) {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/set_humidifier_sleeping_time?value=" + humSleeping, true);
                xhr.send();
            } else {
                alert("Вкажіть затримку між переваркою в межах від 1 до 120");
            }
        }

        function sendTemperatureData() {
            var tempValue = temperature.value;
            if (tempValue >= 1 && tempValue <= 120) {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/set_temperature?value=" + tempValue, true);
                xhr.send();
            } else {
                alert("Вкажіть затримку між переваркою в межах від 1 до 120");
            }
        }

        function sendHumidityData() {
            var humidityValue = humidity.value;
            if (humidityValue >= 1 && humidityValue <= 100) {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/set_humidity?value=" + humidityValue, true);
                xhr.send();
            } else {
                alert("Вкажіть рівень вологості в межах від 1 до 100");
            }
        }

        function toggleTemperature() {
            var xhr = new XMLHttpRequest();
            var temp = document.getElementById("temperatureValue");
            xhr.open("GET", "/checkTemperature", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    var valueT = xhr.responseText;
                    temp.innerText = valueT;
                }
            };
            xhr.send();
        }

        function toggleHumidifier() {
            var xhr = new XMLHttpRequest();
            var humid = document.getElementById("humidityValue");
            xhr.open("GET", "/checkHumidity", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    var valueH = xhr.responseText
                    humid.innerText = valueH;
                }
            };
            xhr.send();
        }

        function tempCheck() {
            var xhr = new XMLHttpRequest();
            var tempCheck = document.getElementById("tempCheck");
            xhr.open("GET", "/tempDelayCheck", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    var valueT2 = parseInt(xhr.responseText, 10) / 1000;
                    tempCheck.innerText = valueT2;
                }
                else {
                    tempCheck.innerText = 5;
                }
            };
            xhr.send();
        }

        function humidCheck() {
            var xhr = new XMLHttpRequest();
            var humidCheck = document.getElementById("humidCheck");
            xhr.open("GET", "/humidTimeCheck", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    var valueH = parseInt(xhr.responseText, 10) / 1000;
                    humidCheck.innerText = valueH;
                }
                else { humidCheck.innerText = 20; }
            };
            xhr.send();
        }

        function delayHumid() {
            var xhr = new XMLHttpRequest();
            var delayHumid = document.getElementById("delayHumid");
            xhr.open("GET", "/humidDelayCheck", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    var valueHD = parseInt(xhr.responseText, 10) / 1000;
                    delayHumid.innerText = valueHD;
                } else {
                    delayHumid.innerText = 10;
                }
            };
            xhr.send();
        }

        function humidLevel() {
            var xhr = new XMLHttpRequest();
            var humidLevel = document.getElementById("humidLevel");
            xhr.open("GET", "humidityLevel", true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    var valueHL = parseInt(xhr.responseText, 10);
                    humidLevel.innerText = valueHL;
                } else {
                    humidLevel.innerText = 10;
                }
            };
            xhr.send();
        }

        setInterval(toggleTemperature, 1000);
        setInterval(toggleHumidifier, 1000);
        setInterval(tempCheck, 1000);
        setInterval(humidCheck, 1000);
        setInterval(delayHumid, 1000);
        setInterval(humidLevel, 1000);
    </script>
</body>

</html>
)rawliteral";

#endif // INDEX_HTML_H