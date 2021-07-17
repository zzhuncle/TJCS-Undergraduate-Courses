Morris.Area({
    element: 'morris-area-chart2',
    data: [{
        period: '2010',
        SiteA: 0,
        SiteB: 0,

    }, {
        period: '2011',
        SiteA: 130,
        SiteB: 100,

    }, {
        period: '2012',
        SiteA: 80,
        SiteB: 60,

    }, {
        period: '2013',
        SiteA: 70,
        SiteB: 200,

    }, {
        period: '2014',
        SiteA: 180,
        SiteB: 150,

    }, {
        period: '2015',
        SiteA: 105,
        SiteB: 90,

    }, {
        period: '2016',
        SiteA: 250,
        SiteB: 150,

    }],
    xkey: 'period',
    ykeys: ['SiteA', 'SiteB'],
    labels: ['Site A', 'Site B'],
    pointSize: 0,
    fillOpacity: 0.4,
    pointStrokeColors: ['#b4becb', '#85b4d0'],
    behaveLikeLine: true,
    gridLineColor: '#e0e0e0',
    lineWidth: 0,
    smooth: false,
    hideHover: 'auto',
    lineColors: ['#b4becb', '#85b4d0'],
    resize: true

});
