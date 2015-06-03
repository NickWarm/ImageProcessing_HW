var pBar = document.getElementbyID('p')
var updateProgress= function(value){
  pBar.value=value
  pBar.getElementByTagName('span')[0].innerHTML=Math.floor((515/70)*value)
}