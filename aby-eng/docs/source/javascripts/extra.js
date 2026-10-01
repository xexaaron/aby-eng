
// When clicking github repo url, open in new tab

document.addEventListener("DOMContentLoaded", () => {
    const links = document.querySelectorAll('a[href*="github.com/xexaaron/aby-eng"]');

    for (const link of links) {
        link.target = "_blank";
        link.rel = "noopener noreferrer";
    }
});