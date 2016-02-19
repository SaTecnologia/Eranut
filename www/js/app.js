var isWebView;
var isAndroid;
angular.module('starter', ['ionic', 'ngCordova'])
.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    if(window.cordova && window.cordova.plugins.Keyboard) {
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
    }
    if(window.StatusBar) {
      StatusBar.styleDefault();
    }


   // var deviceInformation = ionic.Platform.device();

  isAndroid = ionic.Platform.isAndroid();
  // var isIPad = ionic.Platform.isIPad();
  // var isIOS = ionic.Platform.isIOS();
  // var isAndroid = ionic.Platform.isAndroid();
  // var isWindowsPhone = ionic.Platform.isWindowsPhone();
  // var currentPlatform = ionic.Platform.platform();
  // var currentPlatformVersion = ionic.Platform.version();
  //ionic.Platform.exitApp(); // stops the app
  });
})
.controller('AppCtrl', [
  '$scope',
  '$cordovaBluetoothSerial',
  '$ionicModal',
  '$ionicLoading',
  '$timeout',
  '$ionicPlatform',

  function (
    $scope,
    $cordovaBluetoothSerial,
    $ionicModal,
    $ionicLoading,
    $timeout,
    $ionicPlatform
  ) {

    $scope.conectado  = false;
    $scope.habilitado = false;
    $scope.bluetooths = [];
    $scope.config = {};
    $scope.data  = {};



    

    // $scope.checkBT = function (time) {
    //   $timeout(function () {
    //     $cordovaBluetoothSerial.isEnabled().then(fun,fun);
    //    },time); 
    // };
    // $scope.checkBT(750);



    $ionicPlatform.ready(function() {
        if(isAndroid) {
          $cordovaBluetoothSerial.isEnabled().then(function(){
              $scope.habilitado = true;
             
              $scope.listarBluetooth();
            },function(){
             
              $scope.habilitado = false;
          });
        }
    });

    $scope.hide = function(){ $ionicLoading.hide(); };
    $scope.carregando = function(msg) {
      $ionicLoading.show({ template: msg + " <br/><br/><ion-spinner icon='ripple'></ion-spinner>",  showBackdrop: true, delay:500 });
    };

    $scope.conectarBluetooth = function(blue) {
      $scope.conectar(blue.bluetooth);
      console.log(blue);
      $scope.data  = {};
    };

    // $scope.resetDiasSemana = () => {
    //   $scope.diasSemana = [
    //     { nome: "Segunda",  checked: false, intervalo: "" },
    //     { nome: "Terça",    checked: false, intervalo: "" },
    //     { nome: "Quarta",   checked: false, intervalo: "" },
    //     { nome: "Quinta",   checked: false, intervalo: "" },
    //     { nome: "Sexta",    checked: false, intervalo: "" },
    //     { nome: "Sabado",   checked: false, intervalo: "" },
    //     { nome: "Domingo",  checked: false, intervalo: "" }
    //   ];
    // };

    // $scope.resetDiasSemana();



    $scope.intervalos = [
      { nome: '15 minutos', value: 0 },
      { nome: '20 minutos', value: 1 },
      { nome: '25 minutos', value: 2 },
      { nome: '30 minutos', value: 3 },
      { nome: '35 minutos', value: 4 },
      { nome: '40 minutos', value: 5 },
      { nome: '45 minutos', value: 6 },
      { nome: '50 minutos', value: 7 },
      { nome: '55 minutos', value: 8 },
      { nome: '60 minutos', value: 9 }
    ];

    $scope.dias = [
      { nome: 'Domingo', value: 0 },
      { nome: 'Segunda-feira', value: 1 },
      { nome: 'Terça-feira', value: 2 },
      { nome: 'Quarta-feira', value: 3 },
      { nome: 'Quinta-feira', value: 4 },
      { nome: 'Sexta-feira', value: 5 },
      { nome: 'Sabado', value: 6 }
    ];

    $scope.meses = [
      { nome: 'Janeiro', value: 0},
      { nome: 'Fevereiro', value: 0},
      { nome: 'Março', value: 0},
      { nome: 'Abril', value: 0},
      { nome: 'Maio', value: 0},
      { nome: 'Junho', value: 0},
      { nome: 'Julho', value: 0},
      { nome: 'Agosto', value: 0},
      { nome: 'Setembro', value: 0},
      { nome: 'Outubro', value: 0},
      { nome: 'Novembro', value: 0},
      { nome: 'Dezembro', value: 0}
    ];
    
    $scope.anos = [];

    for(var i=2014; i<= new Date().getFullYear(); i++ ) {
      $scope.anos.push({ano: i});
    };

    $scope.horas = [];

    for(var i=0; i<= 24; i++ ) {
      $scope.horas.push({hora: i});
    };
    
    $scope.minutos = [];

    for(var i=0; i<= 60; i++ ) {
      $scope.minutos.push({minuto: i});
    };

    $scope.gravarHora = function(config) {
      $scope.data = angular.copy(config);
      $scope.config = {};
    };

    $ionicModal.fromTemplateUrl('configuracao.html', {
      scope: $scope,
      animation: 'slide-in-up'
    }).then(function(modal) {
      $scope.modal = modal;
    });
    $scope.openModal = function() {
      $scope.modal.show();
      $scope.config = {};
    };
    $scope.closeModal = function() {
      $scope.modal.hide();
      $scope.config = {};
    };
    //Cleanup the modal when we're done with it!
    $scope.$on('$destroy', function() {
      $scope.modal.remove();
    });
    // Execute action on hide modal
    $scope.$on('modal.hidden', function() {
      // Execute action
    });
    // Execute action on remove modal
    $scope.$on('modal.removed', function() {
      // Execute action
    });

    $scope.listarBluetooth = function(){
      $scope.carregando('Listando bluetooths..');
      
      $cordovaBluetoothSerial.list().then(
        function(results) {
          $ionicLoading.hide();
          angular.forEach(results, function(paireddev){
            $scope.bluetooths.push(paireddev);
          });
        },
        function(error) {
          $ionicLoading.hide();
          alert(error);
        }
      );
    };

    $scope.conectar = function(address){
      $scope.carregando('Conectando...');
      
      $cordovaBluetoothSerial.connect(address).then(function(){
        $cordovaBluetoothSerial.isConnected().then(function(){
          $scope.conectado = true;
          $ionicLoading.hide();
        }, function(err){
          $scope.conectado = false;
          $ionicLoading.hide();
          alert(err);
        });
      },function(err){
        $scope.conectado = false;
        $ionicLoading.hide();
        alert(err);
      });
    }


    $scope.gravar = function(item) {
      $scope.carregando('Enviando dados...');
      
      $cordovaBluetoothSerial.isConnected().then(function(){
        var segunda = item.segunda  ? '1' : '0';
        var terca   = item.terca    ? '1' : '0';
        var quarta  = item.quarta   ? '1' : '0';
        var quinta  = item.quinta   ? '1' : '0';
        var sexta   = item.sexta    ? '1' : '0';
        var sabado  = item.sabado   ? '1' : '0';
        var domingo = item.domingo  ? '1' : '0';

        var enviar = segunda+terca+quarta+quinta+sexta+sabado+domingo+'?';

        console.log('Enviando: ');
        console.log(enviar);

        
        $cordovaBluetoothSerial.write(enviar).then(function(){
          console.log('Sucesso ao enviar ' + enviar);
          $ionicLoading.hide();
        }, function(err){
          console.log(err);
          $ionicLoading.hide();
        });
      }, function() {
        $ionicLoading.hide();
        alert('Não conectado');
      });
    }
}]);