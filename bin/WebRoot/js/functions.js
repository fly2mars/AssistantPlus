function reloadFrame(iframe, src) { iframe.src = src; }
function iframe_reload(iframeId, url){
  reloadFrame(document.getElementById(iframeId), url)
}

