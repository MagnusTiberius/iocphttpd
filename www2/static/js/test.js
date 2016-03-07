"use strict";
/**
 * Innehåller defintionen för {@link marginalCalc}
 * @author Hanif Bali
 * @file
 * @version 0.3.5
 */

/**
 * Detta är hjärtat i kalkylatorn, här laddar du och kör dina uträkningar.
 * @namespace marginalCalc
 */
var marginalCalc = {
    loaded: false,
    /**
     * Kalkylatorn använder olika "moduler" för att räkna ut olika bidrag eller skatter. Skapa gärna egna moduler.
     * @interface  Module
     */

    /**
     * Moduler som vill påverka summan av {@link marginalCalc.totalCalc} måste ha denna metod,
     * den returnerar antingen ett positivt eller negativt tal, ett bidrag eller avdrag ger tex ett positivt tal,
     * medan en skatt genererar ett negativt tal.
     * @name Module.#totalCalc
     * @method
     * @returns {Number}
     */

    /**
     * Array med alla moduler registrerade av {@link marginalCalc.addModule}
     * @type {Module[]}
     */
    modules: [],
    modulePriority: [],
    moduleCache: [],

    /**
     * Registrerar modulen för att kunna bli kallad av kalkylatorn. Den känner
     * även av om modulen har en totalCalc funktion som kan köras senare för att
     * räkna ihop summan av samtliga moduler.
     *
     * @example var nyaModulen = function() {
     *              this.totalCalc = function() {
     *                  return -200;
     *              }
     *          }
     * {@link marginalCalc}.addModule("Nymodul", nyaModulen, 200)
     *             .addModule("Modul2",
     *                         function() {
     *                          this.totalCalc = function() {
     *                              return 500;
     *                          }
     *                         },201);
     *
     * marginalCalc.totalCalc() //-> 300
     * @param {String} moduleName name of the Module, referenced function parameters
     * @param {Module} module The module function
     * @param {Number} [priority = 100] Defines in what order the priority the module shall be calculated.
     * @returns {marginalCalc} Den returnerar sig själv för att kunna kedja anrop.
     */

    addModule: function (moduleName, module, priority) {

        this.modules[moduleName] = module;
        var tempObj = new module;

        if (tempObj.hasOwnProperty("totalCalc")) {
            this.modulePriority.push({'priority': priority ? priority : 100, 'moduleName': moduleName});
        }

        return this;
    },
    /**
     * Denna metod sparar den medskickade funktionen och väntar på att scriptloadern ska ge grönt ljus.
     * Denna metod hittar automatiskt vilka moduler funktionen vill
     * ha tillgång till genom att scanna alla argument till funktionen, argumenten ska heta samma sak som
     * man angivit som moduleName till {@link marginalCalc.addModule}.
     * Den initierar automatiskt de moduler som funktionen är beroende av. Den initerar även modulernas beroenden.
     * När den initerat klart anropar den först funktionen och därefter metoden ready() på funktionen om den är definierad.
     * Det kan vara så att man inte behöver lägga sin kod i ready() då alla moduler bör vara laddade vid körning.
     *
     * @example marginalCalc.run(function(socialBidrag){
     *      var soc = {@link socialBidrag}.totalCalc(); //-> ger socialbidraget;
     *      this.ready = function(){
     *          var readySoc = {@link socialBidrag}.totalCalc()
     *          console.log(readySoc === soc); //-> true
     *      }
     * });
     *
     * @param {function} fn Funktionen som ska köras
     */
    run: function (func) {
        this.modules = [];
        this.modules["$Main"] = func;
        this.waitForLoad();
    },
    /**
     * Waits for nod from scriptloader.
     * @private
     */
    waitForLoad: function () {
        var that = this;
        if (this.scriptLoader.complete === true) {

            this.runMain(this.modules["$Main"]);
            return;
        }
        console.log("skipping");
        this.timeout = setTimeout(function () {
            that.waitForLoad()
        }, 1000);
    },
    /**
     * Runs the main function, may be used to skip the nod from {Scriptloader}.
     * @private
     * @param fn
     * */
    runMain: function (fn) {
        this.execModule("$Main", fn);

        if (this.moduleCache["$Main"].hasOwnProperty("ready")) {
            this.moduleCache["$Main"].ready();
        }

    },
    /**
     * Initiates the function, injects the arguments defined fromm the list of loaded modules
     * it also saves the initiated function.
     * @param moduleName Name of the module its loading
     * @param func function body of the module
     * @private
     * @returns {Object} returns the Module
     */
    execModule: function (moduleName, func) {

        if (typeof this.moduleCache[moduleName] !== "undefined") {
            return this.moduleCache[moduleName];
        }

        var obj = new func;
        var dependencies = this.resolveDependencies(func);
        func.apply(obj, dependencies);
        return this.moduleCache[moduleName] = obj;

    },
    /**
     * Finds and loads the dependecies of the function
     * @param func Function to find the dependecies of
     * @private
     * @returns {Array} of modules
     */
    resolveDependencies: function (func) {
        var args = this.getArguments(func);
        var dependencies = [];
        for (var i = 0; i < args.length; i++) {
            if (args[i].length) {
                if (typeof this.modules[args[i]] === "undefined") {
                    throw new Error("Module (" + args[i] + ") is not registered, requested by module");
                }
                dependencies.push(this.execModule(args[i], this.modules[args[i]]));
            }

        }
        return dependencies;
    },
    /**
     * Extracts the arguments from a function
     * @param func
     * @private
     * @returns {Array} of arguments
     */
    getArguments: function (func) {
        //This regex is from require.js
        var FN_ARGS = /^function\s*[^\(]*\(\s*([^\)]*)\)/m;
        var args = func.toString().match(FN_ARGS)[1].split(',');
        args = args.map(function (arg) {
            return arg.trim();
        });
        return args;
    },
    /**
     * Kör metoden {@link Module.totalCalc} på varenda registrerad modul som har den metoden, den kör metoden
     * i den prioritet som man definierade när man registrerade modulen.
     * Antingen får man tillbaka en array med resultatet per modul, eller totalsumman.
     * @param {Boolean} [returnArr = false] False returnerar summan, true returnerar en array med separerade resultat per modul.
     *
     * @returns {Number|Array} Total sum of modules.
     */
    totalCalc: function (returnArr) {

        if (returnArr) {
            return this.$totalCalc();
        } else {
            return this.$totalCalc()["totalCalc"];
        }


    },
    /**
     * Returnerar resultatet av samtliga modulers totalCalc() i en array, använd totalCalc(true) istället.
     * @returns {Array}
     * @private
     */
    $totalCalc: function () {
        var totalCalc = [];

        var totalCalcModules = this.modulePriority.sort(function (a, b) {
            return a.priority - b.priority
        });

        var totalSum = 0;
        for (var i = 0; i < totalCalcModules.length; i++) {
            var moduleName = totalCalcModules[i].moduleName;
            var module = this.execModule(moduleName, this.modules[moduleName]);
            totalSum += totalCalc[moduleName] = module.totalCalc(totalSum);
        }
        totalCalc["totalCalc"] = totalSum;
        return totalCalc;

    },
    /**
     * Räknar ut kvoten av förändringen.
     * @example // Räkna ut marginaleffekter på hyreshöjning med 100 kr från 0 - 7000.
     * household.getHouse().setRent(0); // sätt hyran på noll
     *
     * var hyreshojning = 100;
     * var oldval = marginalCalc.totalCalc(); // ta reda på hur mkt man får från början i bidrag.
     *
     * // Börja mäta förändringar
     *      for(var i = 1; i < 7000 / increment; i++){
     *
     *          household.getHouse().setRent(i * hyreshojning); //100 kr -> 7000 kr
     *          var newVal = marginalCalc.totalCalc(); // få resultat av ex bostadsbidrag och socialbidrag
     *          console.log(marginalCalc.marginalEff(hyreshojning, newVal, oldVal)) // få marginaleffekten
     *          oldVal = newVal // spara den nuvarande summan för att jämföra med loop.
     *       }
     * @param increment Ökningen av att mäta marginaleffekten på
     * @param newVal Det nya värdet
     * @param oldVal Det tidigare värdet
     * @param notInverted inverterar resultatet, 0.75 blir 0.25
     * @returns {Number}
     */
    marginalEff: function (increment, newVal, oldVal, notInverted) {

        if (notInverted) {
            return ((newVal - oldVal) / increment) * 100;
        }
        return (1 - ((newVal - oldVal) / increment)) * 100;

    },


    config: {
        /**
         * Url för scriptloader, ändra denna om du tex vill en annan url för dina filer.
         * @memberof! marginalCalc
         * @default
         * @alias marginalCalc.config.url
         * @type {String}
         */
        url: "",
        /**
         * Namnet på mappen för dina moduler.
         * @memberof! marginalCalc
         * @default
         * @alias marginalCalc.config.url
         * @type {String}
         */
        moduleFolder: "modules/"
    },

    scriptLoader: {
        complete: true,
        promisedFiles: [],
        loaded: [],
        /**
         * Denna funktion bör anropas av varje fil som ska laddas av addScripts. Om inte detta görs så kör kommer kalkylatorn inte börja initiera modulerna.
         * @example // /modules/mina_nya_moduler.js
         * var nyaModulen = function() {
         *      this.totalCalc = function() {
         *          return -200;
         *      }
         * }
         * marginalCalc.addModule("NyModul", nyaModulen);
         * marginalCalc.scriptLoader.loadComplete("mina_nya_moduler");
         * @param {String} file Name of the file that has loaded
         * @alias marginalCalc.scriptLoader.loadComplete
         * @memberof! marginalCalc
         */
        loadComplete: function (file) {
            this.loaded.push(file);
            if (this.promisedFiles.length === this.loaded.length) {
                this.complete = true;
            }
        },
        /**
         * Laddar in javascript filer för användning i marginalCalc
         * @example marginalCalc.scriptLoader.addScripts(["socialbidrag", "bostadsbidrag"])
         * // -> laddar /modules/socialbidrag.js och /modules/bostadsbidrag.js
         * @alias marginalCalc.scriptLoader.addScripts
         * @memberof! marginalCalc
         * @param {Array} modules An array of filenames to load from the modules folder
         */
        addScripts: function (files) {
            this.promisedFiles = files;
            this.complete = false;
            var that = this;
            for (var i = 0; i < files.length; i++) {
                var file = files[i];
                var fileref = document.createElement('script');
                fileref.type = "text/javascript";
                fileref.src = marginalCalc.config.url + marginalCalc.config.moduleFolder + file + ".js";
                fileref.async = true;
                document.getElementsByTagName("head")[0].appendChild(fileref);
            }
        }
    }
};